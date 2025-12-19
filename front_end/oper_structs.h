#ifndef OPER_STRUCTS_H
#define OPER_STRUCTS_H

#include "front_end_properties.h"

struct oper_info_t
{
	oper_t oper;
	char* oper_str;
	bool convertible;
	char* std_oper_str;
};


#endif