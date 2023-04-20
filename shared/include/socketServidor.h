#ifndef SOCKET_SERVIDOR_H
#define SOCKET_SERVIDOR_H

#include "standard.h"
#include "commons.h"


int iniciar_servidor(char* ip, char* puerto);
int esperar_cliente(int socket_servidor);
void recibir_mensaje(int socket_cliente, t_log* logger);
void* recibir_buffer(int* size, int socket_cliente);
int recibir_operacion(int socket_cliente);
t_list* recibir_paquete(int socket_cliente);

#endif