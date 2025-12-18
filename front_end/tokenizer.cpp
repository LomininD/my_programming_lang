#include "tokenizer.h"
#include <assert.h>

static bool is_space(char c);
static char* skip_spaces(char* text_buf);
static char* get_number(char* text_buf_pos, node* new_token);


err_t tokenize_text_buf(char* text_buf, token_array_t token_arr[])
{
	assert(text_buf);
	assert(token_arr);

	printf_debug_msg("tokenize_text_buf: began_process\n");

	char* text_buf_pos = skip_spaces(text_buf);
	printf_debug_msg("%s\n", text_buf_pos);

	while (*text_buf_pos != '\0')
	{
		node* new_token = NULL;
		text_buf_pos = get_number(text_buf_pos, new_token); 
	}

	printf_debug_msg("tokenize_text_buf: ended process\n");
	return ok;
}


char* get_number(char* text_buf_pos, node* new_token)
{


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