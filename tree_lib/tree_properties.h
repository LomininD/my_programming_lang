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
    NUM,
    KEY,
};

union data_t 
{
    int number;
    int oper;
    char* word;
};

struct node
{
    node_t type;
    data_t data;
    int code;
    int line;
    node* parent;
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