#include "cpu_utils.h"

int main(int argc, char**argv) {
	if(argc < 2){
		log_error(logger,"Cantidad incorrecta de parametros");
		return EXIT_FAILURE;
	}
	iniciar_logger_CPU();
	pthread_mutex_init(&mutex_interrupt,NULL);
	iniciar_config(argv[1]);
	iniciar_cliente_memoria();
	createTLB();
	pthread_t thread_dispatch;
	pthread_t thread_interrupt;
	
	pthread_create(&thread_dispatch,NULL, (void*)iniciar_cpu_server_dispatch, NULL);
	pthread_detach(thread_dispatch);
	
	pthread_create(&thread_interrupt,NULL, (void*)iniciar_cpu_server_interrupt, NULL);
	pthread_join(thread_interrupt,NULL);

	// Libero Memoria
    log_destroy(logger);
	config_destroy(config);
	pthread_mutex_destroy(&mutex_interrupt);
    return 0;
}
