#ifndef FRONT_END_PROPERTIES_H
#define FRONT_END_PROPERTIES_H

#include "../debug_lib/debug.h"

struct file_data_t
{
	char* input_file_name;  // needs to be freed
	FILE* input_file_ptr;
	char* text_buf;			// needs to be freed
	size_t text_buf_size;
};

#define CHECK_FOR_ERR(VAR){											\
	if (VAR != ok) 													\
	{																\
		printf_log_abortion("main: aborting due to error\n", NULL); \
		return 1;													\
	}																\
}

#endif