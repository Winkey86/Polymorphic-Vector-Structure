#pragma once
#include <stdio.h>

typedef enum {
    VEC_INIT,
    GET_ELEM,
    VEC_FILL,
    VEC_FREE,
    SET_ELEM,
    VEC_PRINTF,
    VEC_FPRINTF,
    SET_CONST_VEC,
    VEC_SUM,
    VEC_SCAL_MULT,
    CUR_VECTORS_FPRINTF,
    UNKNOWN_CMD
} cmd;

static void value_parser(types* type_info, const char* str, void* dest);
cmd cmd_parser(const char *cmd_str);
void menu();  
