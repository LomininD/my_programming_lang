#ifndef FRONT_END_PROPERTIES_H
#define FRONT_END_PROPERTIES_H

enum oper_t
{
	ERR,
	EOS = 1,
	OPEN_PAR,
	CLOSE_PAR,
	COMMA,
	BLOCK_BEGIN,
	BLOCK_END,
	TAKE,
	IN_QUANTITY_OF,
	ADD,
	SUB,
	PORTIONS_OF,
	DILUTED_BY,
	SEASONED,
	COOK_UNTIL,
	IF,
	OTHERWISE_IF,
	OTHERWISE,
	SERVE,
	BUY,
	TASTE,
	BON_APPETIT,
	IS_LESS_THAN,
	IS_LESS_OR_EQUAL_TO,
	IS_MORE_THAN,
	IS_MORE_OR_EQUAL_TO,
	IS_EQUAL_TO,
	IS_NOT_EQUAL_TO,
	FROM,
	RECIPE,

	oper_count
};

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