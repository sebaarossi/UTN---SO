#include "kernel_utils.h"


/***************** SERVIDOR CONSOLA-KERNEL *****************/
void iniciar_kernel_server(){

    t_config* config_2 = config_create("./cfg/ip_propia.config");
    
    IP_KERNEL = config_get_string_value(config_2,"IP_KERNEL");

    
    int socket_kernel_server = iniciar_servidor(IP_KERNEL,get_puerto_escucha());

    //config_destroy(config_2);
    log_info(logger, "Kernel listo para recibir");
    
    while(1){
        int cliente_fd = esperar_cliente(socket_kernel_server);
        serve_kernel_client(&cliente_fd);
    }

}

void serve_kernel_client(int* socket_cliente){
    int cod_op = recibir_operacion(*socket_cliente);
	if(cod_op == -1){
		pthread_exit(NULL);
    }

	process_request(cod_op,*socket_cliente);
}


void process_request(int cod_op, int socket_cliente) {
    t_list* lista_de_instrucciones = recibir_paquete(socket_cliente);


    int* tamanio =  (int*) list_remove(lista_de_instrucciones,0);

    double estimacion_rafaga= get_estimacion_inicial();   
    

    int* socket = malloc(sizeof(int));
    *socket = socket_cliente;
    
    list_add_in_index(lista_sockets,indice_proceso,socket);
    
   
	// Creo el PCB
    t_pcb* pcb = crear_PCB(indice_proceso, *tamanio, lista_de_instrucciones, estimacion_rafaga);
    indice_proceso++;
    
    log_info(logger,"Proceso con id %d ingreso a la cola de new",pcb->id_proceso);

    inicializar_lista_suspension(pcb);
    pthread_mutex_lock(&mutex_new);
    list_add(cola_new,pcb);
    pthread_mutex_unlock(&mutex_new);

    sem_post(&sem_largo_plazo);
}
void inicializar_lista_suspension(t_pcb* pcb){
    
    t_suspension* suspension = malloc(sizeof(t_suspension));

    suspension->id_proceso = pcb->id_proceso;
    suspension->contador = 0;

    list_add(estado_suspendidos,suspension);

}


void imprimirPCB(t_pcb* pcb){
    printf("Id proceso:%d \n",pcb->id_proceso);
    printf("Tamanio: %d\n",pcb->tamanio);
    list_iterate(pcb->instrucciones,imprimir);
    printf("PC: %d\n",pcb->pc);
    printf("Ultima rafaga: %lf\n",pcb->ultima_rafaga);
    printf("estimacion: %lf\n",pcb->estimacion_rafaga);
    printf("estado: %d\n",pcb->estado);
}



void recalcular_estimaciones(t_pcb* pcb){
    
    pcb->estimacion_rafaga_anterior = pcb->estimacion_rafaga_anterior * (1-get_alpha()) + get_alpha() * pcb->ultima_rafaga_real;
    //log_info(logger,"Nueva estimacion del proceso: %f",pcb->estimacion_rafaga_anterior);
    pcb->estimacion_rafaga = pcb->estimacion_rafaga_anterior;
    pcb->ultima_rafaga = 0;
    pcb->ultima_rafaga_real = 0;
}

//////////////////////////////////////////////////////////

/***************** CLIENTE CPU-KERNEL *****************/

void iniciar_cliente_cpu(){ // solo dispatcher
    socket_cpu_dispatch = crear_conexion(get_ip_cpu(),get_puerto_dispatch());
    socket_cpu_interrupt = crear_conexion(get_ip_cpu(),get_puerto_interrupt());
    if(socket_cpu_dispatch != -1)
        printf("Kernel listo para enviar procesos \n");
    else {   
        printf("Error al conectar");
        exit(1);
    }
    
    while(1){

            int cod_op = recibir_operacion(socket_cpu_dispatch);
            t_pcb* pcb;
            int* tiempo_bloqueo;

            switch(cod_op){
                    case EXIT_CPU: 
                                    pcb = recepcionar_paquete_pcb(socket_cpu_dispatch);
                                    pthread_mutex_lock(&mutex_flag_exec);
                                    flag_exec = 0;
                                    pthread_mutex_unlock(&mutex_flag_exec);

                                    sem_post(&sem_multiprogramacion);
                                    cambiar_estado(pcb,_EXIT); 
                                    int* socket = list_get(lista_sockets,pcb->id_proceso);
                                    

                                    // Enviar paquete a memoria para que libere.
                                    t_paquete* paquete_memoria = crear_paquete_con_codigo_especial(FIN_PRO); 
                                    agregar_a_paquete(paquete_memoria, &(pcb->id_proceso), sizeof(int));
                                    enviar_paquete(paquete_memoria, socket_memoria);
                                    eliminar_paquete(paquete_memoria);

                                    //int cod_op = recibir_operacion(socket_cpu_dispatch);
                                    //if(cod_op != -1)
                                        //log_info(logger,"Salida Exitosa");
	                                
                                    log_info(logger,"Proceso con id %d: Exec -> Exit",pcb->id_proceso);
                                    enviar_mensaje("Proceso terminado",*socket);
                                    liberar_conexion(*socket);
                                    eliminar_PCB(pcb);
                                    sem_post(&sem_corto_plazo);
                                    sem_post(&sem_corto_plazo_FIFO);

                                    break;
                    case IO_CPU:
                                    recepcionar_paquete_IO(socket_cpu_dispatch, &tiempo_bloqueo, &pcb);
                                    pthread_mutex_lock(&mutex_flag_exec);
                                    flag_exec = 0;
                                    pthread_mutex_unlock(&mutex_flag_exec);

                                    if(algoritmo==SRT){
                                        recalcular_estimaciones(pcb);
                                    }
                                    
                                    cambiar_estado(pcb,BLOCKED); 

                                    pthread_mutex_lock(&mutex_blocked);
                                    list_add(cola_suspended_times,tiempo_bloqueo);
                                    list_add(cola_blocked,pcb);
                                    pthread_mutex_unlock(&mutex_blocked);

                                    log_info(logger,"Proceso con id %d: Exec -> Blocked",pcb->id_proceso);

                                    sem_post(&sem_corto_plazo);
                                    sem_post(&sem_mediano_plazo);
                                    sem_post(&sem_corto_plazo_FIFO);
                                    break;
                    case INTERRUPT: 
                                    pcb = recepcionar_paquete_pcb(socket_cpu_dispatch);
                                    log_info(logger,"Proceso con id %d: Exec -> Ready (Interrupcion)",pcb->id_proceso);
                                    
                                    cambiar_estado(pcb,READY);
                                    pcb->estimacion_rafaga = pcb->estimacion_rafaga - pcb->ultima_rafaga;
                                   // printf("Estimacion rafaga del proceso del proceso interrumpido con ID %d es: %f\n",pcb->id_proceso,pcb->estimacion_rafaga);
                                    
                                    pthread_mutex_lock(&mutex_ready);
                                    list_add(cola_ready,pcb);
                                    t_pcb* pcb_con_menor_tiempo = obtener_proceso_con_menor_estimacion(); 
                                    cambiar_estado(pcb_con_menor_tiempo,EXEC);
                                    //printf("Estimacion rafaga del proceso con menor estimacion con ID %d es: %f\n",pcb_con_menor_tiempo->id_proceso,pcb_con_menor_tiempo->estimacion_rafaga);
                                    enviar_paquete_pcb(pcb_con_menor_tiempo);
                                    log_info(logger,"Proceso con id %d: Ready -> Exec",pcb_con_menor_tiempo->id_proceso);
                                    pthread_mutex_unlock(&mutex_ready);
                                    pthread_mutex_lock(&mutex_flag_exec);
                                    flag_exec = 1;
                                    pthread_mutex_unlock(&mutex_flag_exec);
                                    //sem_post(&recibi_interrupt); // Para que no me desaparezca un proceso si llega uno a new cuando estoy ejecutndo un interrupt.
                                  
                                    break;
            }
            
    }
}

int recepcionar_op_code(t_list_iterator* iterator){
    int* op_code             = (int*) list_iterator_next(iterator);
    int devolucion = *op_code;
    free(op_code);
    return devolucion;
}
