#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include"planificador_largo_plazo.h"
#include"planificador_mediano_plazo.h"
#include"planificador_corto_plazo.h"

#include "configKernel.h" 
#include "pcb.h"
#include "kernelInit.h"

#include <pthread.h>
#include <semaphore.h>


//#define IP_KERNEL "127.0.0.1"

// Defino colas
void recalcular_estimaciones(t_pcb* pcb);
void iniciar_kernel_server();
void process_request(int cod_op, int socket_cliente);
void serve_kernel_client(int* socket_cliente);
//void planificador_corto_plazo();
bool mayor_prioridad(void* primer_proceso,void* segundo_proceso);
double proxima_rafaga(t_pcb* proceso);
int recepcionar_op_code(t_list_iterator* iterator);
void blocked_process_for_n_time();
t_pcb* recepcionar_pcb_cpu(t_list_iterator* iterator);
void imprimirPCB(t_pcb* pcb);
void inicializar_lista_suspension(t_pcb* pcb);

#endif