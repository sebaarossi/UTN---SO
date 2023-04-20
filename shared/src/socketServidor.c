#include "socketServidor.h"

int iniciar_servidor(char* ip, char* puerto) {
	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	// Asociamos el socket a un puerto
	if(setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEPORT) failed");

	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}

int esperar_cliente(int socket_servidor) {
	// Aceptamos un nuevo cliente
	struct sockaddr_in dir_cliente;
	int tam_dir = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*)&dir_cliente, (socklen_t*)&tam_dir);

	return socket_cliente;
}


void recibir_mensaje(int socket_cliente, t_log* logger) {
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);

    log_info(logger,"Mensaje Recibido: %s", buffer);

	free(buffer);
}


void* recibir_buffer(int* size, int socket_cliente)
{
        void * buffer;

        recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
        buffer = malloc(*size);
        recv(socket_cliente, buffer, *size, MSG_WAITALL);

        return buffer;
}
  

int recibir_operacion(int socket_cliente) {
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else {
		close(socket_cliente);
		return -1;
	}
}

t_list* recibir_paquete(int socket_cliente) {
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size) {
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
