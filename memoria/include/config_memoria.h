#ifndef CONFIG_MEMORIA_H
#define CONFIG_MEMORIA_H

#include "commons.h"
#include <stdlib.h>
#include <stdio.h>

//Defino el algoritmo
#define CLOCK "CLOCK"
#define CLOCK_M "CLOCK-M" //Revisar no me toma el guion


t_config* config;
// Memoria config
int paginas_por_tabla;
int tam_pagina;
int tam_memoria;
int marcos_por_proceso;
int tabla_primer_nivel_global;
int entradas_por_tabla;
char* algoritmo;
int retardo_swap;
int retardo_memoria;


void iniciar_config(char*);

//Falta toda la liberacion de memoria de los char*
char* get_ip_memoria(void);
char* get_puerto_memoria(void);
int get_tam_memoria(void);
int get_tam_paginas(void);
int get_pags_por_tabla(void);
int get_retardo_memoria(void);
char* get_algoritmo_reemplazo(void);
int get_marcos_por_proceso(void);
int get_retardo_swap(void);
char* get_path_swap(void);
int get_entradas_por_tabla();
#endif