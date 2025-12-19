#include "tokenizer.h"
#include "basic_funcs.h"
#include "oper_structs.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

static bool is_space(char c);
static char* skip_spaces(char* text_buf);
static char* get_number(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info);
static char* get_oper(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info);
static char* get_keyword(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info);
static char* get_word(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info);
static char* skip_comment(char* text_buf_pos);
static char* allocate_mem_for_word(size_t size);
static err_t check_token_arr_capacity(token_array_t* token_arr_struct);


#define TEXT_BUF file_data->text_buf
#define FILE_NAME file_data->input_file_name
#define T_ARR token_arr_struct->array
#define ARR_SIZE token_arr_struct->size

err_t tokenize_text_buf(file_data_t* file_data, token_array_t* token_arr_struct)
{
	assert(file_data);
	assert(TEXT_BUF);

	printf_debug_msg("tokenize_text_buf: began_process\n");

	char* text_buf_pos = skip_spaces(TEXT_BUF);
	debug_info_t debug_info = {FILE_NAME, 1, TEXT_BUF};

	while (*text_buf_pos != '\0')
	{
		printf_debug_msg("tokenize_text_buf: left to proceed:\n---\n");
		printf_debug_msg("%s\n---\n", text_buf_pos);

		char* prev_text_buf_pos = text_buf_pos;

		text_buf_pos = get_number(text_buf_pos, token_arr_struct, &debug_info);
		if (text_buf_pos == NULL) return error;

		if (text_buf_pos == prev_text_buf_pos)
		{
			text_buf_pos = get_oper(text_buf_pos, token_arr_struct, &debug_info);
			if (text_buf_pos == NULL) return error;
		}

		if (text_buf_pos == prev_text_buf_pos)
		{
			text_buf_pos = get_keyword(text_buf_pos, token_arr_struct, &debug_info);
			if (text_buf_pos == NULL) return error;
		}

		if (text_buf_pos == prev_text_buf_pos)
		{
			text_buf_pos = get_word(text_buf_pos, token_arr_struct, &debug_info);
			if (text_buf_pos == NULL) return error;
		}

		if (text_buf_pos == prev_text_buf_pos)
		{
			printf_log_err("[from tokenizer][%s:%d:%d] -> failed to recognize token\n", 
				debug_info.file_name, debug_info.current_line, 
				get_current_pos(debug_info.begin_line_ptr, text_buf_pos));
			return error;
		}
	}

	printf_debug_msg("tokenize_text_buf: ended process\n\n");
	return ok;
}

#undef TEXT_BUF
#undef FILE_NAME


#define CUR_LINE debug_info->current_line

char* get_number(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info)
{
	assert(text_buf_pos);

	printf_debug_msg("get_number: began process\n");

	bool is_negative = false;
	int number = 0;
	int number_len = 0;

	while ((*text_buf_pos >= '0' && *text_buf_pos <= '9') || 
								   *text_buf_pos == '-')
	{
		if (*text_buf_pos == '-')
		{
			printf_debug_msg("get_number: got '-'\n");
			if (!is_negative && number_len == 0) is_negative = true;
			else break;
		}
		else
		{
			printf_debug_msg("get_number: got '%c'\n", *text_buf_pos);
			number = number * 10 + (*text_buf_pos - '0');
			number_len++;
		}
		text_buf_pos++;
	}

	if (is_negative) number = -1 * number;

	if (number_len == 0)
	{
		printf_debug_msg("get_number: current token is not a number, "
													"ended process\n");
		if (is_negative) text_buf_pos--;
		return text_buf_pos;
	}

	err_t checked = check_token_arr_capacity(token_arr_struct);
	if (checked != ok) return  NULL;

	fill_node_draft(&T_ARR[ARR_SIZE], NUM, (union data_t){.number = number}, CUR_LINE);
	ARR_SIZE++;

	text_buf_pos = skip_spaces(text_buf_pos);

	printf_debug_msg("get_number: got number %d\n", number);
	printf_debug_msg("get_number: ended process\n\n");
	return text_buf_pos;
}


char* get_oper(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info)
{
	assert(text_buf_pos);

	printf_debug_msg("get_oper: began process\n");
	char oper = *text_buf_pos;

	if (oper == '+' || oper == '-' ||
		oper == '(' || oper == ')' ||
		oper == '<' || oper == '>' ||
		oper == ',' || oper == '\n')
	{	
		err_t checked = check_token_arr_capacity(token_arr_struct);
		if (checked != ok) return NULL;

		fill_node_draft(&T_ARR[ARR_SIZE], OPER, (union data_t){.oper=oper}, CUR_LINE);
		ARR_SIZE++;
		text_buf_pos++;
	}
	else
	{
		printf_debug_msg("get_oper: current token is not an operator, ended process\n\n");
		return text_buf_pos;
	}

	if (oper == '\n')
	{
		printf_debug_msg("get_oper: got oper \\n\n");
		debug_info->current_line++;
		debug_info->begin_line_ptr = text_buf_pos;
	}
	else printf_debug_msg("get_oper: got oper %c\n", oper);

	text_buf_pos = skip_spaces(text_buf_pos);

	printf_debug_msg("get_oper: ended process\n\n");
	return text_buf_pos;
} 


#define FILE_NAME debug_info->file_name
#define LINE_BEGIN debug_info->begin_line_ptr
#define OPER_STR oper_data[i].oper_str
#define STR_LEN oper_data[i].oper_str_len
#define CODE oper_data[i].oper

char* get_keyword(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info)
{
	assert(text_buf_pos);
	assert(token_arr_struct);

	printf_debug_msg("get_keyword: began process\n");

	for (int i = 0; i < (int) oper_count; i++)
	{
		if (strncmp(text_buf_pos, OPER_STR, STR_LEN) == 0)
		{
			err_t checked = check_token_arr_capacity(token_arr_struct);
			if (checked != ok) return NULL;

			char* word = allocate_mem_for_word(STR_LEN + 1);
			if (word == NULL) return NULL;

			word = strcpy(word, OPER_STR);

			fill_node_draft(&T_ARR[ARR_SIZE], KEY, (union data_t){.word = word}, CUR_LINE);
			T_ARR[ARR_SIZE].code = CODE;
			ARR_SIZE++;


			printf_debug_msg("get_word: recognized keyword %s, ended process\n", OPER_STR);
			text_buf_pos += STR_LEN + 1; 
			text_buf_pos = skip_spaces(text_buf_pos);
			return text_buf_pos;
		}
	}

	printf_debug_msg("get_keyword: current token is not a keyword\n");
	printf_debug_msg("get_keyword: ended process\n\n");

	return text_buf_pos;
}

#undef OPER_STR
#undef STR_LEN
#undef CODE


char* get_word(char* text_buf_pos, token_array_t* token_arr_struct, debug_info_t* debug_info)
{
	assert(text_buf_pos);
	assert(token_arr_struct);

	printf_debug_msg("get_word: began process\n");

	// allocating memory for word
	char* word = allocate_mem_for_word(max_word_len);
	if (word == NULL) return NULL;

	int word_size = 0;

	// main loop
	while((*text_buf_pos >= 'A' && *text_buf_pos <= 'Z') ||
		  (*text_buf_pos >= 'a' && *text_buf_pos <= 'z') ||
		  (*text_buf_pos >= '0' && *text_buf_pos <= '9') ||
		   *text_buf_pos == '_' || *text_buf_pos == '!'  || 
		   *text_buf_pos == ':')
	{
		printf_debug_msg("get_word: got %c\n", *text_buf_pos);
		
		// checking for forbidden start of the word
		if ((*text_buf_pos == '!' || *text_buf_pos == ':') && word_size == 0)
		{
			printf_log_err("[from tokenizer][%s:%d:%d] -> failed to recognize token\n", 
					FILE_NAME, CUR_LINE, get_current_pos(LINE_BEGIN, text_buf_pos));
			return NULL;
		}
		word[word_size] = *text_buf_pos;
		word_size++;
		text_buf_pos++;

		// checking is word is too big
		if (word_size == max_word_len)
		{
			printf_log_err("[from tokenizer][%s:%d:%d] -> max word size exceeded\n", 
					FILE_NAME, CUR_LINE, get_current_pos(LINE_BEGIN, text_buf_pos));
			return NULL;
		}
	}

	// checking if we read sth
	if (word_size == 0)
	{
		printf_debug_msg("get_word: current_token is not a word, ended process\n");
		return text_buf_pos;
	}

	printf_debug_msg("get_word: recognized word %s\n", word);
	text_buf_pos = skip_spaces(text_buf_pos);

	// checking for comments
	if (strcmp(word, "note:") == 0)
	{
		printf_debug_msg("get_word: got comment\n");
		text_buf_pos = skip_comment(text_buf_pos);
		printf_debug_msg("get_word: skipped comment, ended process\n\n");
		return text_buf_pos;
	}

	// adding token to array
	err_t checked = check_token_arr_capacity(token_arr_struct);
	if (checked != ok) return NULL;
	fill_node_draft(&T_ARR[ARR_SIZE], WORD, (union data_t){.word=word}, CUR_LINE);
	ARR_SIZE++;

	printf_debug_msg("get_word: ended process\n\n");
	return text_buf_pos;
}

#undef CUR_LINE


char* skip_comment(char* text_buf_pos)
{
	assert(text_buf_pos);

	while(*text_buf_pos != '\n' && *text_buf_pos != '\0')
	{
		text_buf_pos++;
	}
	return text_buf_pos;
}


#define TOKEN_TYPE T_ARR[i].type
#define TOKEN_DATA T_ARR[i].data

void dump_tokens(token_array_t* token_arr_struct)
{
	assert(token_arr_struct);

	printf_log_bold("===TOKEN DUMP===\n\n", NULL);

	for (size_t i = 0; i < ARR_SIZE; i++)
	{
		printf_debug_msg("%zu. line: %d, type: %s, value: ", i, T_ARR[i].line, 
					decode_node_type_enum(TOKEN_TYPE));
		switch(TOKEN_TYPE)
		{
			case NUM:
				printf_debug_msg("%d\n", TOKEN_DATA.number);
				break;
			case OPER: // FIXME - {}
				if (TOKEN_DATA.oper == '\n')
				{
					printf_debug_msg("\\n\n");
				}
				else printf_debug_msg("%c\n", TOKEN_DATA.oper);
				break;
			case WORD:
				printf_debug_msg("%s\n", TOKEN_DATA.word);
				break;
			case KEY:
				printf_debug_msg("%s\n", TOKEN_DATA.word);
				break;
			default:
				break;
		};
	}
	printf_log_bold("\n================\n\n", NULL);
}

#undef TOKEN_TYPE
#undef TOKEN_DATA


#define ARR_CAP  token_arr_struct->capacity

err_t check_token_arr_capacity(token_array_t* token_arr_struct)
{
	assert(token_arr_struct);

	if (ARR_SIZE == ARR_CAP)
	{
		printf_debug_msg("check_token_arr_capacity: reallocating memory "
												"for token_arr\n");
		ARR_CAP *= 2;
		node* temp_token_arr = (node*) realloc(T_ARR, ARR_CAP * sizeof(node));
		if (temp_token_arr == NULL)
		{
			printf_log_err("[from check_token_arr_capacity] -> could not "
									"reallocate memory for token array\n");
			return error;
		}
		T_ARR = temp_token_arr;

		printf_debug_msg("check_token_arr_capacity: memory reallocated successfully, "
													"new capacity: %zu\n", ARR_CAP);
	}
	else
	{
		printf_debug_msg("check_token_arr_capacity: token_arr capacity is ok\n");
	}
	return ok;
}


err_t initialize_token_arr(token_array_t* token_arr_struct)
{
	assert(token_arr_struct);

	printf_debug_msg("initialize_token_arr: began process\n");

	T_ARR = (node*) calloc(min_size_of_token_arr, sizeof(node));
	if (T_ARR == NULL)
	{
		printf_log_err("[from initialize_token_arr] -> could not "
								"allocate memory for token array\n");
		return error;
	}
	ARR_CAP = min_size_of_token_arr;

	printf_debug_msg("initialize_token_arr: allocated memory for token arr and set capacity\n");

	printf_debug_msg("initialize_token_arr: ended process\n\n");
	return ok;
}


void clear_token_arr(token_array_t* token_arr_struct)
{
	assert(token_arr_struct);

	printf_debug_msg("clear_token_arr: began process\n");	

	for (size_t i = 0; i < ARR_SIZE; i++)
	{
		destroy_node_data(&T_ARR[i]);
	}
	free(T_ARR);

	printf_debug_msg("clear_token_arr: freed token data and token array\n");

	printf_debug_msg("clear_token_arr: ended process\n\n");
}

#undef T_ARR
#undef ARR_CAP
#undef ARR_SIZE


#define OPER_STR oper_data[i].oper_str


#undef OPER_STR


char* allocate_mem_for_word(size_t size)
{
	char* word = (char*) calloc(size, sizeof(char));
	if (word == NULL)
	{
		printf_log_err("[allocate_mem_for_word] -> could not allocate memory for word\n");
		return NULL;
	}
	return word;
}


char* skip_spaces(char* text_buf)
{
	assert(text_buf);

	while (is_space(*text_buf)) text_buf++; 
	return text_buf;
}


bool is_space(char c)
{
	if (c == ' '  || c == '\r' ||
		c == '\t' || c == '\v') return true;
	return false;
}