#include "shared_utils.h"

void imprimir(void* instruccion){

	t_instruccion* inst = (t_instruccion*) instruccion;
	printf("%c\n",inst->ID_instruccion);
	printf("%lu, %lu\n",(long int)inst->parametros[0],(long int) inst->parametros[1]);
	printf("%x, %x\n",inst->parametros[0], inst->parametros[1]);

}

char caracter_segun_instruccion(char* inst) {
	if(strcmp(inst,NO_OP) == 0){
		return 'N';
	}

	if(strcmp(inst,IO) == 0){
		return 'I';
	}
	
	if(strcmp(inst,READ) == 0){
		return 'R';
	}
	
	if(strcmp(inst,WRITE) == 0){
		return 'W';
	}
	
	if(strcmp(inst,COPY) == 0){
		return 'C';
	}

	if(strcmp(inst,EXIT) == 0){
		return 'E';
	}

	return EXIT_FAILURE;
	
}


t_instruccion* instruccion_create() {
	t_instruccion  *instruccion = malloc(sizeof(t_instruccion));
	instruccion->ID_instruccion = 0;
	instruccion->parametros[0] = 0;
    instruccion->parametros[1] = 0;
	return instruccion;
}

void element_destroyer(void* elemento){
	free(elemento);
}