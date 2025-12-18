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


#define DATA new_node_ptr->data

node* create_node_draft(node_t type, union data_t data, int line)
{
    node* new_node_ptr = create_node();

    assert(new_node_ptr);

    new_node_ptr->type = type;
    new_node_ptr->line = line;

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
        default:
            return NULL;
    };

    return new_node_ptr;
}

#undef DATA


void destroy_tree(tree* tree_ptr)
{
    assert(tree_ptr != NULL);

    destroy_node(tree_ptr->root);

    // tree_ptr->root = NULL;
    free(tree_ptr);
}


size_t destroy_node(node* node)
{
    if (node == NULL) return 0;

    size_t nodes_destroyed = 0;

    if (node->left != NULL)
    {
        nodes_destroyed += destroy_node(node->left);
    }
    if (node->right != NULL)
    {
        nodes_destroyed += destroy_node(node->right);
    }

    if (node->type == WORD) free(node->data.word); // free other types if required
    free(node);

    nodes_destroyed ++;

    return nodes_destroyed;
}



