#include "syntax_procession.h"
#include <assert.h>

static node* get_func_decl(token_array_t* token_arr_struct, size_t* pos, const char* file_name);
node* get_token_tree(token_array_t* token_arr_struct, const char* file_name);

err_t syntax_err = ok;

err_t link_tokens(token_array_t* token_arr_struct, const char* file_name)
{
	assert(token_arr_struct);

	printf_debug_msg("link_tokens: began process\n");

	node* token_tree_root = get_token_tree(token_arr_struct, file_name);

	printf_debug_msg("link_tokens: ended process\n\n");
	return ok;
}


#define ARR_SIZE token_arr_struct->size

node* get_token_tree(token_array_t* token_arr_struct, const char* file_name)
{
	assert(token_arr_struct);

	printf_debug_msg("get_token_tree: began process\n");

	size_t pos = 0;

	node* connection_node = get_func_decl(token_arr_struct, &pos, file_name);

	if (pos != ARR_SIZE)
	{
		printf_log_err("[from get_token_tree] -> unanalyzed tokens left\n");
		return NULL;
	}

	printf_debug_msg("get_token_tree: ended process\n\n");
	return connection_node;
}

#undef ARR_SIZE


#define CUR_TOKEN token_arr_struct->array[*pos]
#define PREV_TOKEN token_arr_struct->array[(*pos) - 1]
#define ARG_ERR {														\
				printf_log_err("[syntax error][%s:%d] -> wrong " 		\
					"argument listing\n", file_name, CUR_TOKEN.line);	\
				syntax_err = error;										\
				return NULL;											\
}
#define EOS_ERR {														\
				printf_log_err("[syntax error][%s:%d] -> no \n in " 	\
				"the end of the line\n", file_name, CUR_TOKEN.line);	\
				syntax_err = error;										\
				return NULL;											\
}

node* get_func_decl(token_array_t* token_arr_struct, size_t* pos, const char* file_name)
{
	assert(token_arr_struct);

	node* connection_node = NULL;

	printf_debug_msg("get_func_decl: began process\n");

	if (CUR_TOKEN.type != WORD)
		return NULL;
	printf_debug_msg("get_func_decl: recognized name of recipe\n");

	node* name = create_node();
	fill_node_draft(name, WORD, (union data_t){.word = "name"}, -1);
	name->left = &CUR_TOKEN;
	
	(*pos)++;

	printf_debug_msg("get_func_decl: created name node\n");

	if (CUR_TOKEN.code == FROM)
	{
		printf_debug_msg("get_func_decl: recognized argument list\n");
		
		(*pos)++;
		if (CUR_TOKEN.type != WORD) ARG_ERR;

		printf_debug_msg("get_func_decl: recognized argument\n");

		(*pos)++;
		node* prev_node = name;
		while (CUR_TOKEN.code == COMMA)
		{
			prev_node->right = &CUR_TOKEN;
			prev_node = &CUR_TOKEN; // fix it
			(*pos)++;
			if (CUR_TOKEN.type == WORD) prev_node->left = &CUR_TOKEN;
			else ARG_ERR;
			(*pos)++;

			printf_debug_msg("get_func_decl: recognized another argument\n");
		}
		prev_node->left = &PREV_TOKEN;
	}

	if (CUR_TOKEN.code != RECIPE)
	{
		printf_log_err("[syntax error][%s:%d] -> wrong "
					"declaration (no recipe)\n", file_name, CUR_TOKEN.line);
		syntax_err = error;
		return NULL;
	}

	printf_debug_msg("get_func_decl: recognized recipe keyword\n");

	CUR_TOKEN.left = name;
	connection_node = &CUR_TOKEN;

	pos++;

	if (CUR_TOKEN.code != EOS) EOS_ERR;

	printf_debug_msg("get_func_decl: ended process\n\n");
	return connection_node;
}

#undef CUR_TOKEN
#undef ARG_ERR
#undef EOF_ERR
#undef PREV_TOKEN


// node* get_token_tree(token_array_t* token_arr_struct, size_t* pos, char* file_name)
// {
// 	assert(token_arr_struct);
// 
// 	printf_debug_msg("get_token_tree: began process\n");
// 
// 	size_t begin_pos = *pos;
// 
// 	printf_debug_msg("get_token_tree: ended process\n\n");
// }


