#include "memoria.h"
#include "memoria_utils.h"


int main(int argc, char** argv){
    if(argc < 2){
		log_error(logger,"Cantidad incorrecta de parametros");
		return EXIT_FAILURE;
	}
    init_memoria(argv[1]);
	iniciar_memoria_server();
    terminar_memoria();

    return 0;
}

