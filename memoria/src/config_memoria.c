#include "config_memoria.h"

void iniciar_config(char *config_path){
    config = config_create(config_path);
    if(config == NULL){
        printf("Error: El archivo no existe o no se pudo leer");
        exit(EXIT_FAILURE);
    }


    tam_memoria = get_tam_memoria();
    tam_pagina = get_tam_paginas();

    marcos_por_proceso = get_marcos_por_proceso();
    entradas_por_tabla= get_entradas_por_tabla();

    algoritmo= get_algoritmo_reemplazo(); 
    retardo_swap= get_retardo_swap();
    retardo_memoria= get_retardo_memoria();
}

char* get_ip_memoria(){
    char* ip;
    ip = config_get_string_value(config,"IP_MEMORIA");
    return ip;
}

char* get_puerto_memoria(){
    char* puerto;
    puerto = config_get_string_value(config,"PUERTO_ESCUCHA");
    return puerto;
}

int get_tam_memoria(){
    int tam_memoria;
    tam_memoria = config_get_int_value(config,"TAM_MEMORIA");
    return tam_memoria;
}

int get_tam_paginas(){
    int tam_pagina;
    tam_pagina = config_get_int_value(config,"TAM_PAGINA");
    return tam_pagina;
}

int get_pags_por_tabla(){
    int pags_por_tabla;
    pags_por_tabla = config_get_int_value(config,"PAGINAS_POR_TABLA");
    return pags_por_tabla;
}

int get_retardo_memoria(){
    int retardo_memoria;
    retardo_memoria = config_get_int_value(config,"RETARDO_MEMORIA");
    return retardo_memoria;
}


char* get_algoritmo_reemplazo(){
    char* algoritmo_reemplazo;
    algoritmo_reemplazo = config_get_string_value(config,"ALGORITMO_REEMPLAZO");
    if(string_equals_ignore_case(algoritmo_reemplazo,"CLOCK") )
        return CLOCK;
    else 
        return CLOCK_M;
    free(algoritmo_reemplazo);
}


int get_marcos_por_proceso(){
    int marcos_por_proceso;
    marcos_por_proceso = config_get_int_value(config,"MARCOS_POR_PROCESO");
    return marcos_por_proceso;
}

int get_retardo_swap(){
    int retardo;
    retardo = config_get_int_value(config,"RETARDO_SWAP");
    return retardo;
}

char* get_path_swap(){
    char* path_swap;
    path_swap = config_get_string_value(config,"PATH_SWAP");
    return path_swap;
}

int get_entradas_por_tabla(){
    int entradas;
    entradas = config_get_int_value(config,"ENTRADAS_POR_TABLA");
    return entradas;
}