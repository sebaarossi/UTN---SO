#ifndef CONFIGCPU_H
#define CONFIGCPU_H

#include "cpu_utils.h"
#include "commons.h"
#include <stdlib.h>
#include <stdio.h>

#define LRU 1
#define FIFO 2

void iniciar_config(char*);

int get_entradas_tlb();
int get_algoritmo_tlb();
int get_retardo_NOOP();

#endif