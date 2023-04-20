#include "memoria_utils.h"
#include "memoria_cpu.h"
#include "memoria_kernel.h"


/***************** INICIALIZACION MEMORIA *****************/
void init_memoria(char*config_path){
    iniciar_logger_memoria();
    iniciar_config(config_path);
    semaforos_init();
    iniciar_estructuras_paginacion();
    iniciar_memoria_ram(tam_memoria);
}


void iniciar_memoria_server() {
    pthread_t thread_recepcionar_cpu;
	pthread_t thread_recepcionar_kernel;
    t_config* config_2 = config_create("./cfg/ip_propia.config");
    
    IP_MEMORIA = config_get_string_value(config_2,"IP_MEMORIA");

    log_info(logger,"%s",IP_MEMORIA);
    socket_memoria_server = iniciar_servidor(IP_MEMORIA,get_puerto_memoria());

    //config_destroy(config_2);

    if(socket_memoria_server == -1){
        log_error(logger,"No se pudo crear la conexion con el servidor");
        exit(1);
    }

    log_info(logger, "Memoria lista para recibir");
    
    pthread_create(&thread_recepcionar_kernel,NULL,(void*) recepcionar_kernel,NULL);
	pthread_detach(thread_recepcionar_kernel);  

	pthread_create(&thread_recepcionar_cpu,NULL,(void*) recepcionar_cpu,NULL);
    pthread_join(thread_recepcionar_cpu,NULL);
} 


void iniciar_logger_memoria(){
    logger = log_create("./cfg/memoria.log","MEMORIA", true, LOG_LEVEL_INFO);
}

void terminar_memoria(){
    // Libero Memoria
    log_destroy(logger);
	config_destroy(config);
    free(memoria_ram);
    semaforos_destroy();
    liberar_estructuras_paginacion();
}

void iniciar_memoria_ram(int tam_memoria){
    memoria_ram = malloc(tam_memoria);

    if(memoria_ram == NULL){
        log_info(logger,"Unable to allocate memory\n");
    }   
}


void iniciar_estructuras_paginacion() {
    tabla_paginas_primer_nivel = list_create();
    tabla_paginas_segundo_nivel = list_create();
    cantidad_de_marcos_totales = tam_memoria / tam_pagina;
    bitarray_marcos = calloc(cantidad_de_marcos_totales, sizeof(int));

    for(int i = 0; i<cantidad_de_marcos_totales;i++){
        bitarray_marcos[i] = -1;
    }
}

void liberar_estructuras_paginacion() {
    list_destroy(tabla_paginas_primer_nivel);  //globales
    list_destroy(tabla_paginas_segundo_nivel); // globales 
}

void semaforos_init()
{
    sem_init(&inicio_cpu,0,0);
    sem_init(&sem_swap,0,1);
    pthread_mutex_init(&mutex_tabla_paginas,NULL);
}

void semaforos_destroy()
{
    sem_destroy(&inicio_cpu);
    sem_destroy(&sem_swap);
    pthread_mutex_destroy(&mutex_tabla_paginas);
}