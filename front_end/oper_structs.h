#ifndef OPER_STRUCTS_H
#define OPER_STRUCTS_H

#include "front_end_properties.h"

struct oper_info_t
{
	oper_t oper;
	const char* oper_str;
	size_t oper_str_len;
	bool convertible;
	const char* std_oper_str;
};

extern oper_info_t oper_data[];

#endif