#include "configCpu.h"

void iniciar_config(char * config_path){
    
    config = config_create(config_path);
    if(config == NULL){
        printf("Error: El archivo no existe o no se pudo leer");
        exit(EXIT_FAILURE);
    }

    
}

int get_entradas_tlb(){
    int entradas;
    entradas = config_get_int_value(config,"ENTRADAS_TLB");
    return entradas;
}

int get_algoritmo_tlb(){
    char* algoritmo;
    algoritmo = config_get_string_value(config,"REEMPLAZO_TLB");
    if(string_equals_ignore_case(algoritmo,"FIFO"))
        return FIFO;
    else 
        return LRU;
    free(algoritmo);
}

int get_retardo_NOOP(){
    int retardo;
    retardo = config_get_int_value(config,"RETARDO_NOOP");
    return retardo;
}
