#include "tokenizer.h"
#include <assert.h>

static bool is_space(char c);
static char* skip_spaces(char* text_buf);
static char* get_number(char* text_buf_pos, token_array_t* token_arr_struct, int current_line);
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
	int current_line = 1;
	printf_debug_msg("%s\n", text_buf_pos);

	while (*text_buf_pos != '\0')
	{
		char* prev_text_buf_pos = text_buf_pos;
		text_buf_pos = get_number(text_buf_pos, token_arr_struct, current_line);

		if (text_buf_pos == NULL) return error;
		if (text_buf_pos == prev_text_buf_pos)
		{
			printf_log_err("[from tokenizer][%s:%d:%d] -> failed to recognized token\n", FILE_NAME, current_line, ((size_t)text_buf_pos - (size_t)TEXT_BUF)/sizeof(char) + 1);
			return error;
		}

		printf_debug_msg("%s\n", text_buf_pos);
	}

	printf_debug_msg("tokenize_text_buf: ended process\n\n");
	return ok;
}

#undef TEXT_BUF
#undef FILE_NAME


char* get_number(char* text_buf_pos, token_array_t* token_arr_struct, int current_line)
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
		return text_buf_pos;
	}

	err_t checked = check_token_arr_capacity(token_arr_struct);

	if (checked != ok) return  NULL;

	fill_node_draft(&(T_ARR[ARR_SIZE]), NUM, (union data_t){.number = number}, current_line);
	ARR_SIZE++;

	text_buf_pos = skip_spaces(text_buf_pos);

	printf_debug_msg("get_number: got number %d\n", number);
	printf_debug_msg("get_number: ended process\n\n");
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
		printf_debug_msg("%zu. type: %s, value: ", i, 
					decode_node_type_enum(TOKEN_TYPE));
		switch(TOKEN_TYPE)
		{
			case NUM:
				printf_debug_msg("%d\n", TOKEN_DATA.number);
				break;
			case OPER:
				printf_debug_msg("%c\n", TOKEN_DATA.oper);
				break;
			case WORD:
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

		printf_debug_msg("check_token_arr_capacity: memory reallocated successfully\n");
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