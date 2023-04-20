#include "consola.h"
#include "parser.h"

int main(int argc,char** argv) {
	
	t_log* logger = log_create("../consola/cfg/consola.log","CONSOLA LOGS:",true,LOG_LEVEL_INFO);

	if(argc < 3){
		log_error(logger,"Cantidad incorrecta de parametros");
		return EXIT_FAILURE;
	}

	t_config* config = config_create("./cfg/consola.config");
	t_paquete* paquete = crear_paquete();
	int tamanio = atoi(argv[2]);
	FILE* file_instrucciones = fopen(argv[1],"r+b");
	if(file_instrucciones == NULL){
		log_error(logger,"No existe el archivo");
		return EXIT_FAILURE;
	}

	char* ip = config_get_string_value(config, "IP_KERNEL");
	char* puerto = config_get_string_value(config, "PUERTO_KERNEL");

	t_list* lista_de_instrucciones = cargar_lista(file_instrucciones);
	t_list_iterator* iterador = list_iterator_create(lista_de_instrucciones);

	agregar_a_paquete(paquete,&tamanio,sizeof(int));
	while(list_iterator_has_next(iterador)) {
		t_instruccion* instruccion = list_iterator_next(iterador);
		agregar_a_paquete(paquete,instruccion,9);
	}

	list_iterator_destroy(iterador);

	int conexion = crear_conexion(ip, puerto);
	if(conexion == -1) {
		log_error(logger,"No se pudo crear la conexion con el kernel");	
		exit(1);
	}
	log_info(logger,"Conexion establecida con kernel");

	enviar_paquete(paquete,conexion);
	log_info(logger,"Paquete enviado correctamente");
	
	int op_code = recibir_operacion(conexion);
	if(op_code == MENSAJE) {
		recibir_mensaje(conexion,logger);
		log_info(logger,"Salida Exitosa");
	}
	else {
		log_error(logger,"Respuesta de Kernel Incorrecta");	
		exit(1);
	}
	
	fclose(file_instrucciones);
	list_destroy_and_destroy_elements(lista_de_instrucciones,element_destroyer); 
	log_destroy(logger);
	eliminar_paquete(paquete);
	config_destroy(config);
	liberar_conexion(conexion);

	return EXIT_SUCCESS;
}
