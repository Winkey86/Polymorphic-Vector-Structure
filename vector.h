#pragma once
#include <stddef.h>
#include "types.h"
#include <stdio.h>


typedef struct{
    size_t capacity;   
    size_t elem_size;   
    datatype type;      
    types * type_info;
}Vector;

Vector* vec_init(datatype type,size_t capacity); 
void* get_elem(Vector* v, size_t index); 
void vec_free(Vector* v);
void set_elem(Vector* v, size_t index, void* elem);
void vec_fill(Vector* v, char *s );
void vec_printf(Vector* v);
void vec_fprintf(Vector* v, FILE* out);
void set_const_vec(Vector* v, Vector* v1);
Vector* vec_sum(Vector* v1, Vector* v2);
void* vec_scal_mult(Vector* v1, Vector* v2);
static void value_parser(types* type_info, const char* str, void* dest);
