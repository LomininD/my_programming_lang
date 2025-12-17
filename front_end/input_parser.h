#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "front_end_properties.h"

err_t parse_args(int argc, char* argv[], file_data_t* input_file_data);
err_t read_input_file(file_data_t* input_file_data);
void clear_input_file_data(file_data_t* input_file_data);

#endif