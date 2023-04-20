#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include "standard.h"
#include "commons.h"

#define IO "I/O"
#define COPY "COPY"
#define WRITE "WRITE"
#define READ "READ"
#define EXIT "EXIT"
#define NO_OP "NO_OP"


// Defino posibles devoluciones CPU
/*
#define EXIT_CPU 1
#define IO_CPU 2
#define INTERRUPT 3
*/
typedef struct{
    uint32_t parametros[2];
    char ID_instruccion;
}t_instruccion;


t_instruccion* instruccion_create();
char caracter_segun_instruccion(char* );
void imprimir(void*);
void element_destroyer(void* elemento);
void agregar_instruccion(t_list* ,t_instruccion* );

#endif