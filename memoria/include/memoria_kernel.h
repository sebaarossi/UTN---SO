#ifndef MEMORIA_KERNEL_H
#define MEMORIA_KERNEL_H

#include "memoria_utils.h"
#include "archivos_swap.h"
pthread_mutex_t mutex_tabla_paginas;

void* recepcionar_kernel();

void inicializar_proceso(void);
void suspender_proceso(void);
void finalizar_proceso(void);
int crear_estructuras(int,int);
void liberar_marcos(int);

t_list* obtener_entradas_paginas_modificadas(int , int );
int agregar_tabla_primer_nivel(t_pagina_primer_nivel* );
int agregar_tabla_segundo_nivel(t_pagina_segundo_nivel*);

#endif