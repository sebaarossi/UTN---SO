#ifndef ALGORITMOS_SWAP_H
#define ALGORITMOS_SWAP_H

#include "memoria_utils.h"
#include "archivos_swap.h"

int algoritmo_clock(int id_proceso) ;
int algoritmo_clock_modificado(int);

t_list* obtener_tabla_auxiliar(int tabla_primer_nivel_global);
int buscar_marco_libre(int id_proceso);
int buscarUltimaPosSacada(t_list* lista_marcos_disponibles);
void* leer_dato_memoria(uint32_t direc_fisica);
void cambiarUltimaPosicion(t_entrada_segundo_nivel* puntero, t_entrada_segundo_nivel* proxima_posicion);

#endif