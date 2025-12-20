#include "syntax_procession.h"
#include <assert.h>
#include <string>

static node* get_func_decl(token_array_t* token_arr_struct, size_t* pos, const char* file_name);
static node* get_func(token_array_t* token_arr_struct, size_t* pos, const char* file_name);
static node* get_line(token_array_t* token_arr_struct, size_t* pos, const char* file_name);
static node* get_user_func(token_array_t* token_arr_struct, size_t* pos, const char* file_name);
static node* get_token_tree(token_array_t* token_arr_struct, const char* file_name);
static void revert_changes(token_array_t* token_arr_struct, size_t begin_pos, size_t* cur_pos);

err_t syntax_err = ok;

#define CHECK_ERR(RET) if (syntax_err != ok) return RET

err_t link_tokens(token_array_t* token_arr_struct, const char* file_name, tree* token_tree)
{
	assert(token_arr_struct);

	printf_debug_msg("link_tokens: began process\n");

	token_tree->root = get_token_tree(token_arr_struct, file_name);
	CHECK_ERR(error);

	printf_debug_msg("link_tokens: ended process\n\n");
	return ok;
}


#define ARR_SIZE token_arr_struct->size

node* get_token_tree(token_array_t* token_arr_struct, const char* file_name)
{
	assert(token_arr_struct);

	printf_debug_msg("get_token_tree: began process\n");

	size_t pos = 0;

	// pay attention here, CHECK_SIZE?
	node* connection_node = get_line(token_arr_struct, &pos, file_name);
	CHECK_ERR(NULL);

	if (connection_node == NULL)
	{
		connection_node = get_func_decl(token_arr_struct, &pos, file_name);
		CHECK_ERR(NULL);
	}

	if (pos != ARR_SIZE)
	{
		printf_log_err("[from get_token_tree] -> unanalyzed tokens left\n");
		syntax_err = error;
		return NULL;
	}

	printf_debug_msg("get_token_tree: ended process\n\n");
	return connection_node;
}


#define CUR_TOKEN token_arr_struct->array[*pos]
#define PREV_TOKEN token_arr_struct->array[(*pos) - 1]
#define ARG_ERR {														\
				printf_log_err("[syntax error][%s:%d] -> wrong " 		\
					"argument listing\n", file_name, CUR_TOKEN.line);	\
				syntax_err = error;										\
				return NULL;											\
}
#define EOS_ERR {														\
				printf_log_err("[syntax error][%s:%d] -> no \\n in " 	\
				"the end of the line\n", file_name, CUR_TOKEN.line);	\
				syntax_err = error;										\
				return NULL;											\
}
#define NEXT (*pos)++
#define CHECK_SIZE(){														\
	if (*pos >= ARR_SIZE) {													\
		printf_log_err("[syntax error][%s:%d] -> not enough tokens\n",		\
								file_name, PREV_TOKEN.line);				\
		syntax_err = error;													\
		return NULL;														\
	}																		\
}
#define NEXT_T NEXT; CHECK_SIZE()

node* get_func_decl(token_array_t* token_arr_struct, size_t* pos, const char* file_name)
{
	assert(token_arr_struct);

	node* func_node = get_func(token_arr_struct, pos, file_name);
	CHECK_ERR(NULL);

	if (CUR_TOKEN.code != RECIPE)
	{
		printf_log_err("[syntax error][%s:%d] -> wrong "
					"declaration (no recipe)\n", file_name, CUR_TOKEN.line);
		syntax_err = error;
		return NULL;
	}

	printf_debug_msg("get_func_decl: recognized recipe keyword\n");
	CUR_TOKEN.left = func_node;
	func_node = &CUR_TOKEN;

	NEXT_T;

	if (CUR_TOKEN.code != EOS) EOS_ERR;
	CUR_TOKEN.left = func_node;
	node* connection_node = &CUR_TOKEN;

	printf_debug_msg("get_func_decl: recognised eos\n");

	NEXT;

	printf_debug_msg("get_func_decl: ended process\n\n");
	return connection_node;
}


node* get_line(token_array_t* token_arr_struct, size_t* pos, const char* file_name)
{
	assert(token_arr_struct);

	printf_debug_msg("get_line: began process\n");

	size_t begin_pos = *pos;

	bool is_func = false;

	node* line_node = get_user_func(token_arr_struct, pos, file_name);
	CHECK_ERR(NULL);
	CHECK_SIZE();
	printf_debug_msg("get_line: recognized user_func\n");
	is_func = true;

	if (CUR_TOKEN.code != EOS)
	{
		if (is_func)
		{
			printf_debug_msg("get_line: not a line\n");
			revert_changes(token_arr_struct, begin_pos, pos);
			return NULL;
		}
		else EOS_ERR; 
	}

	CUR_TOKEN.left = line_node;
	node* connection_node = &CUR_TOKEN;

	printf_debug_msg("get_line: recognised eos\n");

	NEXT;

	printf_debug_msg("get_line: ended process\n\n");
	return connection_node;
}


node* get_user_func(token_array_t* token_arr_struct, size_t* pos, const char* file_name)
{
	assert(token_arr_struct);

	printf_debug_msg("get_user_func: began process\n");

	node* connection_node = get_func(token_arr_struct, pos, file_name);
	CHECK_ERR(NULL);

	node* name_node = connection_node;
	connection_node = connection_node->left;
	connection_node->left = name_node->right;
	name_node->left = name_node->right = NULL;
	destroy_node(name_node);

	printf_debug_msg("get_user_func: ended process\n\n");
	return connection_node;
}


node* get_func(token_array_t* token_arr_struct, size_t* pos, const char* file_name)
{
	assert(token_arr_struct);

	printf_debug_msg("get_func: began process\n");

	if (CUR_TOKEN.type != WORD)
		return NULL;

	printf_debug_msg("get_func: recognized name of recipe\n");

	node* name = create_node(); // name should be freed specially 
	fill_node_draft(name, FUNC_INFO, (union data_t){.word = strdup("name")}, -1);
	name->left = &CUR_TOKEN;
	
	NEXT_T;

	printf_debug_msg("get_func: created name node\n");

	if (CUR_TOKEN.code == FROM)
	{
		printf_debug_msg("get_func: recognized argument list\n");
		
		NEXT_T;
		if (CUR_TOKEN.type != WORD) ARG_ERR;

		printf_debug_msg("get_func: recognized argument\n");

		NEXT_T;
		node* prev_node = name;

		while (CUR_TOKEN.code == COMMA)
		{
			prev_node->right = &CUR_TOKEN;
			CUR_TOKEN.left = &PREV_TOKEN;
			prev_node = &CUR_TOKEN;
			NEXT_T;
			if (CUR_TOKEN.type != WORD) ARG_ERR;
			NEXT;

			printf_debug_msg("get_func: recognized another argument\n");
		}

		prev_node->right = &PREV_TOKEN;
	}

	printf_debug_msg("get_func: ended process\n\n");
	return name;
}

#undef CUR_TOKEN
#undef ARG_ERR
#undef EOF_ERR
#undef PREV_TOKEN
#undef CHECK_ERR
#undef NEXT
#undef CHECK_SIZE
#undef NEXT_T
#undef ARR_SIZE


#define CUR_TOKEN token_arr_struct->array[i]

void revert_changes(token_array_t* token_arr_struct, size_t begin_pos, size_t* cur_pos)
{
	assert(token_arr_struct);

	for (size_t i = begin_pos; i < *cur_pos; i++)
	{
		CUR_TOKEN.left = NULL;
		CUR_TOKEN.right = NULL;
	}
	*cur_pos = begin_pos;
}

#undef CUR_TOKEN

// node* get_token_tree(token_array_t* token_arr_struct, size_t* pos, const char* file_name)
// {
// 	assert(token_arr_struct);
// 
// 	printf_debug_msg("get_token_tree: began process\n");
// 
// 	size_t begin_pos = *pos;
// 
// 	printf_debug_msg("get_token_tree: ended process\n\n");
//  return connection_node;
// }


