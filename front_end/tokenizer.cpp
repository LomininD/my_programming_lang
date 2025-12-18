#include "tokenizer.h"
#include <assert.h>

static bool is_space(char c);
static char* skip_spaces(char* text_buf);
static char* get_number(char* text_buf_pos, node* new_token, size_t* current_line, const char* file_name);


#define TEXT_BUF file_data->text_buf
#define FILE_NAME file_data->input_file_name

err_t tokenize_text_buf(file_data_t* file_data, token_array_t token_arr[])
{
	assert(file_data);
	assert(TEXT_BUF);
	assert(token_arr);

	printf_debug_msg("tokenize_text_buf: began_process\n");

	char* text_buf_pos = skip_spaces(TEXT_BUF);
	size_t current_line = 1;
	printf_debug_msg("%s\n", text_buf_pos);

	while (*text_buf_pos != '\0')
	{
		node* new_token = NULL;
		text_buf_pos = get_number(text_buf_pos, new_token, &current_line, FILE_NAME);
		// if text_buf_pos == NULL
		printf_debug_msg("%s\n", text_buf_pos);
	}

	printf_debug_msg("tokenize_text_buf: ended process\n");
	return ok;
}

#undef TEXT_BUF
#undef FILE_NAME


char* get_number(char* text_buf_pos, node* new_token, size_t* current_line, const char* file_name)
{
	assert(text_buf_pos);

	bool is_negative = false;

	while ((*text_buf_pos >= '0' && *text_buf_pos <= '9') || 
								   *text_buf_pos == '-')
	{
		if (*text_buf_pos == '-')
		{
			if (!is_negative)
				is_negative = true;
			else
			{
				printf_log_err("[%s:%zu][from tokenizer] -> "
					"extra '-' symbol in number", file_name, *current_line);
				return NULL;
			}
		}
		else
		{
			return NULL; // in progress
		}
	}

	text_buf_pos++;
	text_buf_pos = skip_spaces(text_buf_pos);
	return text_buf_pos;
}


// dump tokens


char* skip_spaces(char* text_buf)
{
	assert(text_buf);

	while (is_space(*text_buf)) text_buf++; 
}


bool is_space(char c)
{
	if (c == ' '  || c == '\r' ||
		c == '\t' || c == '\v') return true;
	return false;
}