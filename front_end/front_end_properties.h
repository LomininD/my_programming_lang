#ifndef FRONT_END_PROPERTIES_H
#define FRONT_END_PROPERTIES_H

#include "../debug_lib/debug.h"
#include "../tree_lib/tree_properties.h"
#include "../tree_lib/tree_funcs.h"

struct file_data_t
{
	char* input_file_name;
	FILE* input_file_ptr;
	char* text_buf;
	size_t text_buf_size;
};

const int min_size_of_token_arr = 100;

struct token_array_t
{
	node* array;
	size_t size;
	size_t capacity;
};

#define IF_ERR(VAR){											\
	if (VAR == true) 													\
		return 1;														\
}

#endif