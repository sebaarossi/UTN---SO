#ifndef CPU_CICLO_INST_H
#define CPU_CICLO_INST_H

#include "cpu_init.h"
#include "mmu.h"
#include <pthread.h>
#include <unistd.h>
#include "pcb.h"
#include "commons.h"
#include "standard.h"
#include <time.h>

void correr_ciclo_instruccion(t_pcb * pcb);
t_instruccion* determinar_siguiente_instruccion (t_pcb * pcb);
void ejecutar_instruccion(t_instruccion* instruccion, t_pcb * pcb);
int get_checkeo_interrupt();
void fetch_operands(t_instruccion* instruccion, t_pcb* pcb);
void ejecutar_instruccion_NOOP();
void ejecutar_instruccion_IO(t_instruccion* instruccion,t_pcb* pcb);
void ejecutar_instruccion_EXIT(t_pcb * pcb);
void ejecutar_instruccion_READ(t_instruccion* instruccion, t_pcb * pcb);
void ejecutar_instruccion_WRITE(t_instruccion* instruccion, t_pcb * pcb);
void ejecutar_instruccion_COPY(t_instruccion* instruccion, t_pcb * pcb, uint32_t valor_copy);

#endif