#ifndef PLANIFICADOR_MEDIANO_PLAZO_H
#define PLANIFICADOR_MEDIANO_PLAZO_H

#include "configKernel.h" 
#include "pcb.h"
#include "kernelInit.h"

#include <pthread.h>
#include <semaphore.h>

void planificador_mediano_plazo();
void controlar_suspension(int* id);
void ejecucion_IO();
t_pcb* estaBloqueado(int id_proceso);
t_suspension* buscar_suspension(int id);
#endif