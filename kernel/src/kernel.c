
#include "kernel_utils.h"

int main(int argc, char**argv) {
	if(argc < 2){
		log_error(logger,"Cantidad incorrecta de parametros");
		return EXIT_FAILURE;
	}

	pthread_t thread_corto_plazo;
	pthread_t thread_mediano_plazo;
	pthread_t thread_largo_plazo;
	pthread_t thread_cpu_dispatch;
	pthread_t thread_kernel_consola;
	pthread_t thread_process_blocked;
	
	init_kernel(argv[1]);
	printf("Kernel initialized\n");	

	cliente_memoria();

	pthread_create(&thread_largo_plazo,NULL,(void*)planificador_largo_plazo,NULL);
	pthread_detach(thread_largo_plazo);

	pthread_create(&thread_mediano_plazo,NULL,(void*)planificador_mediano_plazo,NULL);
	pthread_detach(thread_mediano_plazo);

	pthread_create(&thread_corto_plazo,NULL,(void*)planificador_corto_plazo,NULL);
	pthread_detach(thread_corto_plazo);
	

	pthread_create(&thread_process_blocked,NULL,(void*) ejecucion_IO,NULL);
    pthread_detach(thread_process_blocked);
	
	pthread_create(&thread_kernel_consola,NULL,(void*)iniciar_kernel_server,NULL);
	pthread_detach(thread_kernel_consola);
	
	pthread_create(&thread_cpu_dispatch,NULL,(void*)iniciar_cliente_cpu,NULL);
	pthread_join(thread_cpu_dispatch,NULL);
	

	//pthread_detach(thread_kernel_consola);
	printf("Kernel finalizado\n");
	
	// Libero Memoria
	release_kernel();
    return 0;
}
