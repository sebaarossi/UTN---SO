#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

#include "memoria.h"
#include "config_memoria.h"
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>

//#define IP_MEMORIA "127.0.0.1"

#define FINALIZADO  'F'
#define EN_USO      'U'


// Variables Iniciales
char* IP_MEMORIA;
t_log* logger;
pthread_t thread;

//time_t start_t, end_t;
//habria que hacer dos funciones que esperen el tiempo del config y cuando finaliza que ponga el semaforo en uno
struct timespec start_cpu, stop_cpu;
struct timespec start_swap, stop_swap;


// Variables sockets
int socket_cliente_cpu;
int socket_cliente_kernel;
int socket_memoria_server;

//Semaforos
pthread_mutex_t mutex_bitarray;
sem_t inicio_cpu;
sem_t sem_retardo_swap;
sem_t sem_swap;
//memoria
void *memoria_ram; 


// TABLAS
typedef struct t_entrada_segundo_nivel{
    int marco;
    bool bit_presencia;
    bool bit_de_uso;
    bool bit_de_modificado;
    int indice;
    int pagina;
} t_entrada_segundo_nivel;

typedef struct t_pagina_segundo_nivel{
        int id_proceso;
        t_list* entradas_segundo_nivel;
} t_pagina_segundo_nivel;

typedef struct t_pagina_primer_nivel{
    int id_proceso;
    t_list* entradas_primer_nivel; //Int
} t_pagina_primer_nivel;



t_list* tabla_paginas_primer_nivel;
t_list* tabla_paginas_segundo_nivel;
int cantidad_de_marcos_totales;
int* bitarray_marcos;

//Funciones de inicializacion
void init_memoria(char*);
void iniciar_logger_memoria(void);
void iniciar_memoria_server();
void semaforos_init();
void semaforos_destroy();

void esperar_cliente_memoria(int socket_memoria_server);
void serve_memoria_client(int socket_cliente);

void iniciar_memoria_ram(int);
void crear_memoria_virtual(int );
void terminar_memoria(void);

void iniciar_estructuras_paginacion();
void liberar_estructuras_paginacion();
//void bit_array_create(t_bitarray, int);


#endif