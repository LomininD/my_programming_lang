#include <stdlib.h>
#include <assert.h>
#include "tree_funcs.h"

tree* tree_ctor()
{
    tree* tree_ptr = (tree*) calloc(1, sizeof(tree));

    assert(tree_ptr);

    return tree_ptr;
}


node* create_node()
{
    node* new_node_ptr = (node*) calloc(1, sizeof(node));

    assert(new_node_ptr);

    return new_node_ptr;
}


#define DATA node_ptr->data

void fill_node_draft(node* node_ptr, node_t type, union data_t data, int line)
{
    assert(node_ptr);

    node_ptr->type = type;
    node_ptr->line = line;
    node_ptr->code = -1;

    switch(type)
    {
        case OPER:
            DATA.oper = data.oper;
            break;
        case NUM:
            DATA.number = data.number;
            break;
        case WORD:
            DATA.word = data.word;
            break;
        case KEY:
            DATA.word = data.word;
            break;
        default:
            return;
    };
}

#undef DATA


void destroy_tree(tree* tree_ptr)
{
    assert(tree_ptr != NULL);

    destroy_node(tree_ptr->root);

    // tree_ptr->root = NULL;
    free(tree_ptr);
}


void destroy_node_data(node* node_ptr)
{
    assert(node_ptr != NULL);

    if (node_ptr->type == WORD) free(node_ptr->data.word);
    // free other types if required
}


size_t destroy_node(node* node_ptr)
{
    if (node_ptr == NULL) return 0;

    size_t nodes_destroyed = 0;

    if (node_ptr->left != NULL)
    {
        nodes_destroyed += destroy_node(node_ptr->left);
    }
    if (node_ptr->right != NULL)
    {
        nodes_destroyed += destroy_node(node_ptr->right);
    }

    destroy_node_data(node_ptr);
    free(node_ptr);

    nodes_destroyed ++;

    return nodes_destroyed;
}



