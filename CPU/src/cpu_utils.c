#include "cpu_utils.h"

void iniciar_logger_CPU() {
    logger = log_create("./cfg/cpu.log","CPU", true, LOG_LEVEL_INFO);
}

void iniciar_cliente_memoria() {
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    
    socket_memoria = crear_conexion(ip_memoria, puerto_memoria);
	if(socket_memoria == -1) {
		log_error(logger,"No se pudo crear la conexion con la memoria");	
		exit(1);
	}

	log_info(logger,"Conexion establecida con la memoria");

    handshake_inicial_memoria();
}

void handshake_inicial_memoria() {
    enviar_operacion(INI_CPU, socket_memoria);
	
    recibir_operacion(socket_memoria);
    t_list* lista = recibir_paquete(socket_memoria);

    int* aux_tam_pagina = list_remove(lista,0);
    int* aux_entradas_por_tabla = list_remove(lista,0);

    tam_pagina = *aux_tam_pagina;
    entradas_por_tabla = *aux_entradas_por_tabla;

    list_destroy(lista);
    free(aux_entradas_por_tabla);
    free(aux_tam_pagina);
}


void iniciar_cpu_server_dispatch() {
    char* puertoDispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");

    t_config* config_2 = config_create("./cfg/ip_propia.config");
    
    char* IP_CPU = config_get_string_value(config_2,"IP_CPU");

   
    int socket_cpu_dispatch = iniciar_servidor(IP_CPU,puertoDispatch);

    config_destroy(config_2);

    if(socket_cpu_dispatch == -1) {
        log_error(logger,"No se pudo crear la conexion con el servidor");
        exit(1);
    }
    log_info(logger, "Puerto dispatch listo para recibir\n");
    
    id_proceso_actual = -1;
    int socket_cliente_dispatch = esperar_cliente(socket_cpu_dispatch);

    while(1) {    
        recibir_operacion(socket_cliente_dispatch);
        t_pcb* pcb = recepcionar_paquete_pcb(socket_cliente_dispatch);

        if(id_proceso_actual != pcb->id_proceso ) {
            limpiar_tlb();
            id_proceso_actual = pcb->id_proceso;
        }
        pthread_mutex_lock(&mutex_interrupt);
        chequeo_interrupt = 0;
        pthread_mutex_unlock(&mutex_interrupt);
        correr_ciclo_instruccion(pcb);

        enviar_paquete(paqueteDispatch,socket_cliente_dispatch);

        eliminar_paquete(paqueteDispatch);
        eliminar_PCB(pcb);      
    }        
}

void iniciar_cpu_server_interrupt() {
    char* puertoInterrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");

     t_config* config_2 = config_create("./cfg/ip_propia.config");
    
    IP_CPU = config_get_string_value(config_2,"IP_CPU");

    int socket_cpu_interrupt = iniciar_servidor(IP_CPU,puertoInterrupt);

   // config_destroy (config_2);
    
    log_info(logger, "Puerto interrupt listo para recibir\n");

    int cliente_interrupt = esperar_cliente(socket_cpu_interrupt);
    
    while(1) {
        int op_code = recibir_operacion(cliente_interrupt);
	    if(op_code != MENSAJE) {
		    log_error(logger,"Error, recepcion erronea de interrupcion");	
            exit(1);    
	    }

        recibir_mensaje(cliente_interrupt,logger);
		log_info(logger,"Me interrumpieron");
        pthread_mutex_lock(&mutex_interrupt);
        chequeo_interrupt = 1;
        pthread_mutex_unlock(&mutex_interrupt);
    }   

}

void recibir_paquete_cpu(int socket) {
    recibir_operacion(socket);
    t_list* lista = recibir_paquete(socket);
    
    int* aux_tam_pagina = list_remove(lista,0);
    int* aux_entradas_por_tabla = list_remove(lista,0);

    tam_pagina = *aux_tam_pagina;
    entradas_por_tabla = *aux_entradas_por_tabla;

    free(aux_tam_pagina);
    free(aux_entradas_por_tabla);
    
    list_destroy(lista);
}



// A CPU 4 I/O 2 CPU 3
// B CPU 2 I/O 3 CPU 4

// E inicial = 2

// EA = 4*0.5 + 2*0.5 = 3 1cpu
// EB = 2*0.5 + 2*0.5 = 2 1cpu

// EA = 3 * 0.5 + 3* 0.5 2cpu
// EB = 2*0.5 + 4 * 0.5