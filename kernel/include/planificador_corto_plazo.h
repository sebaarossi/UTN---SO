#ifndef PLANIFICADOR_CORTO_PLAZO_H
#define PLANIFICADOR_CORTO_PLAZO_H

#include "configKernel.h" 
#include "pcb.h"
#include "kernelInit.h"

#include <pthread.h>
#include <semaphore.h>

void planificador_corto_plazo();

t_pcb* obtener_proceso_con_menor_estimacion();
//void planificador_corto_plazo();
void enviar_paquete_pcb(t_pcb* pcb);

#endif