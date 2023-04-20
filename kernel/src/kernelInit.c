#include "kernelInit.h"

/***************** INICIALIZACION KERNEL *****************/
void init_kernel(char* config_path){
    iniciar_logger_kernel();
    iniciar_config(config_path);
    

    ip_memoria = get_ip_memoria();
    puerto_memoria = get_puerto_memoria();
    flag_exec = 0;
    indice_proceso = 0;
    contador_suspension = 0;
    grado_multiprogramacion = get_grado_multiprogramacion();
    algoritmo = get_algoritmo();
    inicializar_semaforos();
    inicializar_listas();
    
}

void iniciar_logger_kernel(){
    logger = log_create("./cfg/kernel.log","KERNEL", true, LOG_LEVEL_INFO);
}

void release_kernel(){
    log_destroy(logger);
	config_destroy(config);
    liberar_conexion(socket_cpu_dispatch);
    liberar_conexion(socket_cpu_interrupt);
    destruir_semaforos();
    destruir_listas();
}

void config_destroy_elements(){
    //TODO
}
//////////////////////////////////////////////////////////
void inicializar_semaforos(){
    pthread_mutex_init(&mutex_new,NULL);
    pthread_mutex_init(&mutex_ready,NULL);
    pthread_mutex_init(&mutex_blocked,NULL);
    pthread_mutex_init(&mutex_suspended_ready,NULL);
    pthread_mutex_init(&mutex_suspended_blocked,NULL);
    pthread_mutex_init(&mutex_suspended_times,NULL);
    pthread_mutex_init(&mutex_flag_exec,NULL);
    sem_init(&sem_corto_plazo_FIFO,0,1);
    sem_init(&sem_kernel,0,1);
    sem_init(&sem_largo_plazo,0,0);
    sem_init(&sem_corto_plazo,0,0);
    sem_init(&recibi_interrupt,0,1);
    sem_init(&sem_multiprogramacion,0,grado_multiprogramacion);
    sem_init(&sem_espera_PCB,0,0);
    sem_init(&sem_io,0,0);
    sem_init(&sem_procesos_en_ready,0,0);
    

}
void destruir_semaforos(){
    pthread_mutex_destroy(&mutex_new);
    pthread_mutex_destroy(&mutex_ready);
    pthread_mutex_destroy(&mutex_blocked);
    pthread_mutex_destroy(&mutex_suspended_ready);
    pthread_mutex_destroy(&mutex_suspended_blocked);
    pthread_mutex_destroy(&mutex_suspended_times);
    pthread_mutex_destroy(&mutex_flag_exec);
    sem_destroy(&sem_corto_plazo_FIFO);
    sem_destroy(&sem_largo_plazo);
    sem_destroy(&sem_corto_plazo);
    sem_destroy(&recibi_interrupt);
    sem_destroy(&sem_multiprogramacion);
    sem_destroy(&sem_espera_PCB);
    sem_destroy(&sem_io);
    sem_destroy(&sem_procesos_en_ready);

}

void inicializar_listas(){
    cola_new = list_create();
    cola_ready = list_create();
    cola_suspended_times = list_create();
    cola_suspended_ready  = list_create();
    cola_blocked = list_create();
    lista_sockets = list_create();
    estado_suspendidos = list_create();
}

void destruir_listas(){
    list_destroy_and_destroy_elements(cola_new,eliminar_PCB);
    list_destroy_and_destroy_elements(cola_ready,eliminar_PCB);
    list_destroy_and_destroy_elements(cola_suspended_ready,eliminar_PCB);
    list_destroy_and_destroy_elements(cola_blocked,eliminar_PCB);
    list_destroy_and_destroy_elements(cola_suspended_times,free);
    list_destroy_and_destroy_elements(estado_suspendidos,free);
}

void cliente_memoria()
{
    char* ip_memoria = get_ip_memoria();
    char* puerto_memoria = get_puerto_memoria();

    socket_memoria = crear_conexion(ip_memoria, puerto_memoria);

	if(socket_memoria == -1) {
		log_error(logger,"No se pudo crear la conexion con la memoria");	
		exit(1);
	}
	log_info(logger,"Conexion establecida con la memoria");
    /*
    t_paquete* paquete_grado_multi = crear_paquete(); 
    agregar_a_paquete(paquete_memoria, &grado_multiprogramacion, sizeof(int));
    enviar_paquete(paquete_grado_multi, socket_memoria);
    eliminar_paquete(paquete_memoria);
    
    enviar_operacion(MULTI, socket_memoria);
    log_info("Envio el Grado de Multi a memoria");
*/
}

