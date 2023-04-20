#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include "cpu_init.h"
#include <pthread.h>
#include <unistd.h>
#include "pcb.h"
#include "commons.h"
#include "standard.h"
#include <time.h>
#include "mmu.h"
#include "cpu_ciclo_inst.h"


void iniciar_logger_CPU();
void iniciar_cpu_server_dispatch();
void iniciar_cpu_server_interrupt();
void esperar_cliente_cpu(int socket_cpu_server);
void serve_cpu_client(int* socket_cliente);

//Conexion con la memoria
void iniciar_cliente_memoria();
void handshake_inicial_memoria();

void fetch_operands(t_instruccion* instruccion, t_pcb* pcb);
void correr_ciclo_instruccion(t_pcb * pcb);
t_instruccion * determinar_siguiente_instruccion (t_pcb * pcb);
void ejecutar_instruccion(t_instruccion* instruccion, t_pcb * pcb);
void ejecutar_instruccion_NOOP(void);
void ejecutar_instruccion_IO(t_instruccion* instruccion,t_pcb * pcb);
void ejecutar_instruccion_READ(t_instruccion*, t_pcb *);
void ejecutar_instruccion_WRITE(t_instruccion*, t_pcb *);
void ejecutar_instruccion_COPY(t_instruccion*, t_pcb *, uint32_t);
void ejecutar_instruccion_EXIT(t_pcb * pcb);

void recibir_paquete_cpu(int socket);

#endif 