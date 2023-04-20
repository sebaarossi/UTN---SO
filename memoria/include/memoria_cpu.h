#ifndef MEMORIA_CPU_H
#define MEMORIA_CPU_H

#include "memoria_utils.h"
#include "archivos_swap.h"
#include "algoritmos_swap.h"
int marco_accedido_reciente;
t_pagina_segundo_nivel* tabla_segundo_nivel_global;


void* recepcionar_cpu();

//Funcinoes de las instrucciones
void pedido_fet_op();
uint32_t buscar_valor_memoria(uint32_t);
void escribir_valor_memoria(uint32_t, uint32_t);
void escribir_dato_memoria(uint32_t , void* );
void pedido_ini_cpu(int);
void acceder_tp_1(void);
void acceder_tp_2(void);
void pedido_de_lectura(void);
void pedido_de_escritura(void);
void pedido_de_copy(void);
t_entrada_segundo_nivel* buscar_entrada(int marco_a_buscar);
int primer_marco_asignado(int id_proceso);

#endif