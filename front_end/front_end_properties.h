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

const int min_size_of_node_arr = 100;

struct token_array_t
{
	node* array[min_size_of_node_arr];
	size_t size;
	size_t capacity;
};

#define CHECK_FOR_ERR(VAR){											\
	if (VAR != ok) 													\
	{																\
		printf_log_abortion("main: aborting due to error\n", NULL); \
		return 1;													\
	}																\
}

#endif