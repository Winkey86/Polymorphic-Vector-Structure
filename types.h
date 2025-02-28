#pragma once
#include <stddef.h>
#include <stdio.h>


typedef enum {
    INT,
    DOUBLE,
    COMPLEX
} datatype;

typedef struct {
    size_t size;
    datatype type;
    void (*sum)(void*, void*, void*);
    void (*mult)(void*, void*, void*);
    void (*scal_mult)(void*, double, void*);
    void (*printf)(void*);
    void (*fprintf)(FILE*, void*);
} types;



typedef struct{
    int value;
}Int;


void sum_int(void* el1, void* el2, void* summa);
void mult_int(void* el1, void* el2, void* product);
void scal_mult_int(void* el, double scal, void* product);
void printf_int(void* el);
void fprintf_int(FILE* f,void* el);




typedef struct
{
 double real;
 double imag;
}complex;

void sum_complex(void* el1, void* el2, void* summa);
void mult_complex(void* el1, void* el2, void* product);
void scal_mult_complex(void* el, double scal, void* product);
void printf_complex(void* el);
void fprintf_complex(FILE* f,void* el);





typedef struct {
    double value;
} Double;

void sum_double(void* el1, void* el2, void* summa);
void mult_double(void* el1, void* el2, void* product);
void scal_mult_double(void* el1, double scal, void* product);
void printf_double(void* el);
void fprintf_double(FILE* f,void* el);



extern types Int_type;
extern types Double_type;
extern types Complex_type;
