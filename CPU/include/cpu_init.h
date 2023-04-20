#ifndef CPU_INIT_H
#define CPU_INIT_H

#include <pthread.h>
#include <unistd.h>
#include "pcb.h"
#include "commons.h"
#include "standard.h"
#include <time.h>

//#define IP_CPU "127.0.0.1"

t_log* logger;

char* IP_CPU;
pthread_t thread;
t_config* config;
t_paquete* paqueteDispatch;
int chequeo_interrupt;

pthread_mutex_t mutex_interrupt;
time_t start_t,end_t;

// Proceso Actual
t_pcb* pcb;
int id_proceso_actual;

//Memoria
int socket_memoria;
uint32_t* valor_copy;

#endif 