#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"


types Int_type = {
    .size = sizeof(int),
    .type = INT,
    .sum = sum_int,
    .mult = mult_int,
    .scal_mult = scal_mult_int,
    .printf = printf_int,
    .fprintf = fprintf_int
};

types Double_type = {
    .size = sizeof(double),
    .type = DOUBLE,
    .sum = sum_double,
    .mult = mult_double,
    .scal_mult = scal_mult_double,
    .printf = printf_double,
    .fprintf = fprintf_double
};

types Complex_type = {
    .size = sizeof(complex),
    .type = COMPLEX,
    .sum = sum_complex,
    .mult = mult_complex,
    .scal_mult = scal_mult_complex,
    .printf = printf_complex,
    .fprintf = fprintf_complex 
};


void sum_int(void* el1, void* el2, void* summa){
    *(int*)summa=*(int*)el1+*(int*)el2;
}
void mult_int(void* el1, void* el2, void* product){
    *(int*)product=*(int*)el1**(int*)el2;
}
void scal_mult_int(void* el, double scal, void* product){
    *(int*)product=(int)(*(int*)el*scal);
}

void printf_int(void* el){
    printf("%d", *(int*)el);
}

void fprintf_int(FILE* f, void* el) {
    fprintf(f, "%d", *(int*)el);
}


void sum_complex(void* el1, void* el2, void* summa) {
    complex* com_el1 = (complex*)el1;
    complex* com_el2 = (complex*)el2;
    complex* suma = (complex*)summa;
    suma->real = com_el1->real + com_el2->real;
    suma->imag = com_el1->imag + com_el2->imag;
}

void mult_complex(void* el1, void* el2, void* product) {
    complex* com_el1 = (complex*)el1;
    complex* com_el2 = (complex*)el2;
    complex* prod = (complex*)product;
    prod->real = com_el1->real * com_el2->real - com_el1->imag * com_el2->imag;
    prod->imag = com_el1->real * com_el2->imag + com_el1->imag * com_el2->real;
}

void scal_mult_complex(void* el, double scal, void* product) {
    complex* com_el = (complex*)el;
    complex* prod = (complex*)product;
    prod->real = com_el->real * scal;
    prod->imag = com_el->imag * scal;
}

void printf_complex(void* el){
    complex* com = (complex*)el;
    printf("(%.3lf + %.3lfi)", com->real, com->imag); 
}

void fprintf_complex(FILE* f, void* el) {
    complex* com = (complex*)el;
    fprintf(f, "(%.3lf + %.3lfi)", com->real, com->imag);
}


void sum_double(void* el1, void* el2, void* summa) {
    *(double*)summa = *(double*)el1 + *(double*)el2;
}

void mult_double(void* el1, void* el2, void* product) {
    *(double*)product = *(double*)el1 * *(double*)el2;
}

void scal_mult_double(void* el1, double scal, void* product) {
    *(double*)product = *(double*)el1 * scal;
}

void printf_double(void* el) {
    printf("%.3lf", *(double*)el);
}

void fprintf_double(FILE* f, void* el) {
    fprintf(f, "%.3lf", *(double*)el);
}

