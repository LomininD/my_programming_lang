#ifndef TREE_PROPERTIES_H
#define TREE_PROPERTIES_H

#include "../debug_lib/debug.h"
#define TREE_VERIFICATION

enum dir_t
{
	left,
	right,
	root,
};

enum node_t
{
    OPER,
    WORD,
    NUM
};

union data_t 
{
    int number;
    char oper;
    char* word; 
};

struct node
{
    node* parent;
    node_t type;
    data_t data;
    int line;
    node* left;
    node* right;
};

struct tree
{
    node* root;
    size_t size;
    err_t err_stat;
};

#endif