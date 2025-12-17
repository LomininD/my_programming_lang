#include "input_parser.h"
#include <assert.h>
#include <string.h>
#include <sys/stat.h>


#define FILE_NAME input_file_data->input_file_name

err_t parse_args(int argc, char* argv[], file_data_t* input_file_data)
{
	assert(argv);
	assert(input_file_data);

	printf_debug_msg("parse_args: began process\n");

	if (argc != 2)
	{
		printf_log_err("[from parse_args] -> no input file name presented\n");
		return error;
	}

	FILE_NAME = argv[1];

	printf_debug_msg("parse_args: recognized file name (%s)\n", FILE_NAME);

	printf_debug_msg("parse_args: ended process\n\n");
	return ok;
}


#define FILE_PTR input_file_data->input_file_ptr
#define TEXT_BUF input_file_data->text_buf
#define TEXT_BUF_SIZE input_file_data->text_buf_size

err_t read_input_file(file_data_t* input_file_data)
{
	assert(input_file_data);
	
	printf_debug_msg("read_input_file: began process\n");
	
	FILE_PTR = fopen(FILE_NAME, "r");
	if (FILE_PTR == NULL)
	{
		printf_log_err("[from read_input_file] -> could not open "
								"the input file (file may not exist)\n");
		return error;
	}
	printf_debug_msg("read_input_file: opened input file\n");

	struct stat file_info = {};
	fstat(fileno(FILE_PTR), &file_info);
	size_t bytes_in_file = (size_t) file_info.st_size;
	printf_debug_msg("read_input_file: bytes in input file: %zu\n", 
														bytes_in_file);

	TEXT_BUF = (char*) calloc(bytes_in_file + 1, sizeof(char));
	if (TEXT_BUF == NULL)
	{
		printf_log_err("[from read_input_file] -> could not allocate "
												"memory for text_buf\n");
		return error;
	}

	size_t bytes_read = fread(TEXT_BUF, sizeof(char), bytes_in_file, FILE_PTR);
	printf_debug_msg("read_input_file: bytes read: %zu\n", bytes_read);

	if (bytes_in_file != bytes_read)
	{
		printf_log_err("[from read_input_file] -> number if bytes in "
		"file (%zu) does not match with number of bytes read (%zu)\n", 
		bytes_in_file, bytes_read);
		return error;
	}

	TEXT_BUF[bytes_in_file] = '\0';
	TEXT_BUF_SIZE = bytes_in_file;

	printf_debug_msg("read_input_file: dumping text_buf:\n\n%s\n\n", TEXT_BUF);

	fclose(FILE_PTR);
	printf_debug_msg("read_input_file: closed input file\n");

	printf_debug_msg("read_input_file: ended process\n\n");
	return ok;
}


void clear_input_file_data(file_data_t* input_file_data)
{
	assert(input_file_data);

	printf_debug_msg("clear_input_file_data: began process\n");

	free(TEXT_BUF);
	printf_debug_msg("clear_input_file_data: text_buf cleared\n");

	printf_debug_msg("clear_input_file_data: ended process\n\n");
}

#undef FILE_NAME
#undef FILE_PTR
#undef TEXT_BUF
#undef TEXT_BUF_SIZE
