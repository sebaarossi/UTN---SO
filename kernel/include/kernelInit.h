#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

#include "configKernel.h" 
#include "pcb.h"

#include <pthread.h>
#include <semaphore.h>
int socket_cpu_dispatch;

int socket_cpu_interrupt;

int socket_memoria;
char* IP_KERNEL;
pthread_mutex_t mutex_new, mutex_ready, mutex_blocked, mutex_suspended_ready, mutex_suspended_blocked,mutex_suspended_times;
pthread_mutex_t mutex_flag_exec;
sem_t sem_corto_plazo_FIFO;
sem_t sem_largo_plazo,sem_mediano_plazo,sem_corto_plazo;
sem_t sem_kernel, sem_io;
sem_t recibi_interrupt,sem_multiprogramacion, sem_espera_PCB;
sem_t sem_procesos_en_ready;
sem_t espera_suspension;
int algoritmo;
int grado_multiprogramacion; 

char* ip_memoria;
char* puerto_memoria;

t_config* config_kernel;

int socket_cpu_dispatch;
int socket_cpu_interrupt;
int indice_proceso;
int flag_exec;
int contador_suspension;

t_list *cola_new,*cola_ready,*cola_blocked;
t_list *cola_suspended_ready,*cola_suspended_times;
t_list* lista_sockets;
t_list *estado_suspendidos;
t_log* logger;
t_config* config;

char* IP_CPU;
char* PUERTO_CPU_DISPATCH;
char* PUERTO_CPU_INTERRUPT;

typedef struct t_suspension{
    int id_proceso;
    int contador;
}t_suspension;

void init_kernel(char*);
void init_config_kernel();
void iniciar_logger_kernel();
void inicializar_semaforos();
void iniciar_cliente_cpu();
void destruir_semaforos();
void destruir_listas();

void inicializar_listas();
void cliente_memoria(); 
void release_kernel();
#endif