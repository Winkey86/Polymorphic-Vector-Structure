# Polymorphic-Vector-Structure
Polymorphic vector stucture for INT, DOUBLE and COMPLEX on C language

#RUN CODE
gcc -c types.c -o types.o
gcc -c vector.c -o vector.o
gcc -c menu.c -o menu.o
gcc -c main.c -o main.o
gcc types.o vector.o menu.o main.o -o program
Get-Content -Raw input.txt | .\program.exe
