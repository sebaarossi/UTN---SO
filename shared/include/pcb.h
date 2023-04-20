#ifndef PCB_H
#define PCB_H

#include "shared_utils.h"
#include "socketCliente.h"
#include "socketServidor.h"

// Defino estados posibles

#define NEW                 0
#define READY               1
#define EXEC                2
#define BLOCKED             3
#define SUSPENDED_READY     4
#define SUSPENDED_BLOCKED   5
#define _EXIT               6

// !!!
// Falta paginacion
typedef struct t_pcb{
	int id_proceso;
    int tamanio;
    // Instrucciones Proceso
    t_list* instrucciones;
    // Program Counter
    int pc;
    //Tabla de Paginacion
    int tabla_paginas;
    // Estimacion Proxima Rafaga
    double estimacion_rafaga;
    double estimacion_rafaga_anterior;
    // Estado Proceso
    char estado;
    //Ultima rafaga
    double ultima_rafaga;
    double ultima_rafaga_real;
} t_pcb;


t_pcb* crear_PCB(int id_proceso, int tamanio, t_list* instrucciones, double estimacion_rafaga);
t_pcb* armar_PCB(int id_proceso, int tamanio, t_list* instrucciones, double estimacion_rafaga, int pc, char estado,double ultima_rafaga,double ultima_rafaga_real,int tabla_paginas);
void eliminar_PCB(void* pcb);
int size_PCB( t_pcb *pcb);
void cambiar_estado(t_pcb* pcb, char estado);

t_paquete* armar_paquete_pcb( t_pcb *pcb);
void agregar_a_paquete_pcb(t_paquete* paquete, t_pcb* pcb);
t_pcb* recepcionar_paquete_pcb(int socket);
void recepcionar_paquete_IO(int socket, int** tiempo_bloqueo, t_pcb** pcb);

#endif