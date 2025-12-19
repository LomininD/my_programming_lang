#include "input_parser.h"
#include "front_end_properties.h"
#include "tokenizer.h"


bool check_for_err(err_t res, file_data_t* input_file_data);
bool check_for_err(err_t res, file_data_t* input_file_data, token_array_t* tokens);


int main(int argc, char* argv[])
{
	initialize_log("front_end.html", "FRONT END LOG");
	
	file_data_t input_file_data = {};

	err_t parsed = parse_args(argc, argv, &input_file_data);
	IF_ERR(check_for_err(parsed, &input_file_data));

	err_t file_is_read = read_input_file(&input_file_data);
	IF_ERR(check_for_err(file_is_read, &input_file_data));

	token_array_t tokens = {};
	err_t initialised = initialize_token_arr(&tokens);
	IF_ERR(check_for_err(initialised, &input_file_data, &tokens));

	err_t tokenized = tokenize_text_buf(&input_file_data, &tokens);
	IF_ERR(check_for_err(tokenized, &input_file_data, &tokens));

	dump_tokens(&tokens);

	//err_t linked = link_tokens(&tokens);



	clear_token_arr(&tokens);
	clear_input_file_data(&input_file_data);
	return 0;
}


bool check_for_err(err_t res, file_data_t* input_file_data)
{
	if (res != ok)
	{
		clear_input_file_data(input_file_data);
		printf_log_abortion("main: aborting due to error\n", NULL);
		return true;
	}
	return false;
}


bool check_for_err(err_t res, file_data_t* input_file_data, token_array_t* tokens)
{
	if (res != ok)
	{
		clear_input_file_data(input_file_data);
		clear_token_arr(tokens);
		printf_log_abortion("main: aborting due to error\n", NULL);
		return true;
	}
	return false;
}
