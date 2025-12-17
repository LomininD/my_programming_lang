#include "input_parser.h"
#include "front_end_properties.h"

int main(int argc, char* argv[])
{
	initialize_log("front_end.html", "FRONT END LOG");
	
	file_data_t input_file_data = {};

	err_t parsed = parse_args(argc, argv, &input_file_data);
	CHECK_FOR_ERR(parsed);

	err_t file_is_read = read_input_file(&input_file_data);
	CHECK_FOR_ERR(file_is_read);

	clear_input_file_data(&input_file_data);
	return 0;
}