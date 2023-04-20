#ifndef ARCHIVOS_SWAP_H
#define ARCHIVOS_SWAP_H

#include "memoria_utils.h"
#include <time.h>



void crear_archivo_swap(int,int);
void inicializar_swap(FILE*);
void borrar_archivo_swap(int);
char* crear_path_swap(int );
void* leer_swap(int, uint32_t );
void escribir_en_swap( int, void*, int );



#endif