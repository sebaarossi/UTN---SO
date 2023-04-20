#include "commons.h"
#include <stdlib.h>
#include <stdio.h>

// Defino los planificadores

#define FIFO 1
#define SRT 2

t_config* config;

void iniciar_config(char*);

//Falta toda la liberacion de memoria de los char*
char* get_ip_memoria(void);
char* get_puerto_memoria(void);
char* get_ip_cpu(void);
char* get_puerto_dispatch(void);
char* get_puerto_interrupt(void);
char* get_puerto_escucha(void);
int get_algoritmo(void);
double get_estimacion_inicial(void);
double get_alpha(void);
int get_grado_multiprogramacion(void);
int get_tiempo_maximo_bloqueado(void);