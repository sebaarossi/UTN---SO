#include "configKernel.h"

void iniciar_config(char* config_path){
    
    config = config_create(config_path);
    if(config == NULL){
        printf("Error: El archivo no existe o no se pudo leer");
        exit(EXIT_FAILURE);
    }


}

char* get_ip_memoria(){
    char* ip;
    ip = config_get_string_value(config,"IP_MEMORIA");
    return ip;
}

char* get_puerto_memoria(){
    char* puerto;
    puerto = config_get_string_value(config,"PUERTO_MEMORIA");
    return puerto;
}

char* get_ip_cpu(){
    char* ip;
    ip = config_get_string_value(config,"IP_CPU");
    return ip;
}

char* get_puerto_dispatch(){
    char* puerto;
    puerto = config_get_string_value(config,"PUERTO_CPU_DISPATCH");
    return puerto;
}

char* get_puerto_interrupt(){
    char* puerto;
    puerto = config_get_string_value(config,"PUERTO_CPU_INTERRUPT");
    return puerto;
}

char* get_puerto_escucha(){
    char* puerto;
    puerto = config_get_string_value(config,"PUERTO_ESCUCHA");
    return puerto;
}

int get_algoritmo(){
    char* algoritmo;
    algoritmo = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
    if(string_equals_ignore_case(algoritmo,"FIFO"))
        return FIFO;
    else 
        return SRT;
    free(algoritmo);
}


double get_estimacion_inicial(){
    double estimacion;
    estimacion = config_get_double_value(config,"ESTIMACION_INICIAL");
    return estimacion;
}


double get_alpha(){
    double alpha;
    alpha = config_get_double_value(config,"ALFA");
    return alpha;
}

int get_grado_multiprogramacion(){
    int grado_multiprogramacion;
    grado_multiprogramacion = config_get_int_value(config,"GRADO_MULTIPROGRAMACION");
    return grado_multiprogramacion;
}


int get_tiempo_maximo_bloqueado(){
    int tiempo;
    tiempo = config_get_int_value(config,"TIEMPO_MAXIMO_BLOQUEADO");
    return tiempo;
}