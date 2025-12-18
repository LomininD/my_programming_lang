#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "front_end_properties.h"
#include "../tree_lib/tree_properties.h"
#include "../tree_lib/tree_funcs.h"

err_t tokenize_text_buf(file_data_t* file_data, token_array_t token_arr[]);

#endif