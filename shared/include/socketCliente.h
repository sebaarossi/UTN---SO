#ifndef SOCKET_CLIENTE_H
#define SOCKET_CLIENTE_H

#include "standard.h"
#include "commons.h"


typedef enum {
	DEFAULT=-2,
	ERROR=-1,
	MENSAJE,
	PAQUETE,
	EXIT_CPU, 
	IO_CPU,
    INTERRUPT,
	INI_CPU,
	INI_PRO,
	SUS_PRO,
	FIN_PRO,
	ACC_TAB_1,
	ACC_TAB_2,
	PED_LEC,
	PED_ESC,
	PED_COPY,
	FET_OP,
	MULTI,
} op_code;

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


int crear_conexion(char *ip, char* puerto);
void liberar_conexion(int socket_cliente);

void enviar_mensaje(char* mensaje, int socket_cliente);
void* serializar_paquete(t_paquete* paquete, int bytes);
void eliminar_paquete(t_paquete* paquete);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
t_paquete* crear_paquete_con_codigo_especial(op_code codigo_operacion);
void enviar_operacion(op_code operacion, int socket);

#endif