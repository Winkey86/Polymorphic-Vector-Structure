#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "types.h"
#include <string.h>

extern types Int_type;
extern types Double_type;
extern types Complex_type;



static void value_parser(types* type_info, const char* str, void* dest) {
    if (type_info->type == INT) {
        *(int*)dest = atoi(str);
    } else if (type_info->type == DOUBLE) {
        *(double*)dest = atof(str);
    } else if (type_info->type == COMPLEX) {
        complex* c = dest;
        sscanf(str, "%lf,%lf", &c->real, &c->imag);
    }
}

Vector * vec_init(datatype type, size_t capacity){
    Vector * v = malloc(sizeof(Vector));
    if (!v) {
        fprintf(stderr, "MEMORY ERROR\n");
        return NULL;
    } 
    v->capacity = capacity;
    if (v->capacity == 0) v->capacity = 1;
    v->type = type;
    switch (type) {
        case INT:    v->type_info = &Int_type;    break;
        case DOUBLE: v->type_info = &Double_type;  break;
        case COMPLEX: v->type_info = &Complex_type; break;
        default:
            fprintf(stderr, "ERROR: UNKNOWN TYPE");
            free(v);
            return NULL;
    }
    v->elem_size = v->type_info->size; 
    v->data = malloc(v->elem_size * v->capacity);
    if (!v->data) {
        fprintf(stderr, "VECTOR DATA ERROR\n");
        free(v);
        return NULL;
    }
    return v;
}

void vec_printf(Vector *v){
    if (!v) {
        fprintf(stderr, "MEMORY ERROR\n");
        return;
    } 
    printf("[");
    for (size_t i = 0; i < v->capacity; i++) {
        if (i > 0) printf(", ");
        v->type_info->printf(get_elem(v, i));
    }
    printf("]\n");
}


void vec_fprintf(Vector* v, FILE *out) {
    if (!v) {
        fprintf(out, "VECTOR ERROR\n");
        return;
    }
    if (!out) {
        fprintf(stderr, "FILE OUTPUT ERROR\n");
        return;
    }

    fprintf(out, "[");
    for (size_t i = 0; i < v->capacity; i++) {
        if (i > 0) fprintf(out, ", ");
        v->type_info->fprintf(out, get_elem(v, i));
    }
    fprintf(out, "]\n");
}

void* get_elem(Vector *v, size_t i) {
    if (i >= v->capacity) {
        fprintf(stderr, "INDEX ERROR\n");
        return NULL;
    }
    return (char*)v->data + i * v->type_info->size;
}


void set_elem(Vector *v, size_t i, void* elem){
    if (i>=v->capacity){
        fprintf(stderr, "INDEX ERROR\n");
        return;
    }
    memcpy((char*)v->data + i * v->type_info->size, elem, v->type_info->size);
}

void set_const_vec(Vector *v, Vector *v1) {
    if (v->capacity != v1->capacity || v->type_info->type != v1->type_info->type) {
        fprintf(stderr, "VECTOR ERROR\n");
        return;
    }
    memcpy(v->data, v1->data, v->capacity * v->type_info->size); 
}

Vector* vec_sum(Vector *v, Vector *v1){
    if (v->capacity!=v1->capacity){
        fprintf(stderr, "VECTOR SIZE ERROR\n");
        return NULL;
    }
    if (v->type_info->type!=v1->type_info->type){
        fprintf(stderr, "VECTOR TYPE ERROR\n");
        return NULL;
    }
    Vector* new_vec = vec_init(v->type_info->type, v->capacity);
    void* change = malloc(v->type_info->size);

    for (size_t i = 0; i < v->capacity; i++) {
        v->type_info->sum(get_elem(v, i),get_elem(v1, i),change);
        set_elem(new_vec, i, change);
    }

    free(change);
    return new_vec;
}

void* vec_scal_mult(Vector *v, Vector *v1){
    if (v->capacity != v1->capacity){
        fprintf(stderr, "VECTOR SIZE ERROR\n");
        return NULL;
    }
    if (v->type_info->type != v1->type_info->type){
        fprintf(stderr, "VECTOR TYPE ERROR\n");
        return NULL;
    }
    void* final = calloc(1, v->type_info->size);
    void* product = malloc(v->type_info->size);

    for (size_t i = 0; i < v->capacity; i++) {
        v->type_info->mult(get_elem(v, i), get_elem(v1, i), product);
        v->type_info->sum(final, product, final);
    }

    free(product);
    return final;
}

void vec_free(Vector* v) {
    if (v) {
        free(v->data);
        free(v);
    }
}

void vec_fill(Vector* v, char *s) {
    char *saveptr; 
    for (size_t i = 0; i < v->capacity; i++) {
        char *token = strtok_r(i == 0 ? s : NULL, " ", &saveptr);
        if (!token) break;
        value_parser(v->type_info, token, (char*)v->data + i * v->type_info->size);
    }
}