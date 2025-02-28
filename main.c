#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"   
#include "types.h"
#include "menu.h" 


//=======RUN_CODE==============

/*
gcc -c types.c -o types.o
gcc -c vector.c -o vector.o
gcc -c menu.c -o menu.o
gcc -c main.c -o main.o
gcc types.o vector.o menu.o main.o -o program
Get-Content -Raw input.txt | .\program.exe
*/


int main(){
    menu();
}