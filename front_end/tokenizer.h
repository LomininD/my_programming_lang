#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "front_end_properties.h"
#include "../tree_lib/tree_properties.h"
#include "../tree_lib/tree_funcs.h"

const int max_word_len = 100;

struct debug_info_t
{
	const char* file_name;
	int current_line;
	char* begin_line_ptr;
};


err_t tokenize_text_buf(file_data_t* file_data, token_array_t* token_arr);
void dump_tokens(token_array_t* token_arr_struct);
err_t initialize_token_arr(token_array_t* token_arr_struct);
void clear_token_arr(token_array_t* token_arr_struct);

#endif