#ifndef MMU_H
#define MMU_H

#include <unistd.h>
#include "pcb.h"
#include "commons.h"
#include "standard.h"
#include <math.h>
#include "cpu_utils.h"
#include "tlb.h"

int tam_pagina, entradas_por_tabla;

uint32_t traducir_direccion(uint32_t direccion_logica,t_pcb* pcb);

int obtener_tabla_segundo_nivel(int tabla_paginas, int entrada_tabla_1er_nivel);
int obtener_marco_memoria(int tabla_segundo_nivel, int entrada_tabla_2do_nivel,uint32_t);

#endif