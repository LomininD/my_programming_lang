#include "basic_funcs.h"

int get_current_pos(char* begin_line_ptr, char* current_ptr)
{
	return (int)(((size_t)current_ptr - (size_t)begin_line_ptr) / sizeof(char)) + 1;
}