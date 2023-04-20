#include "pcb.h"


t_pcb* crear_PCB(int id_proceso, int tamanio, t_list* instrucciones, double estimacion_rafaga) {
    t_pcb* pcb = malloc(sizeof(t_pcb));

    pcb->id_proceso = id_proceso;
    pcb->tamanio = tamanio;
    pcb->instrucciones = instrucciones;
    pcb->pc = 0;
    pcb->tabla_paginas = 0;
    pcb->ultima_rafaga = 0;
    pcb->ultima_rafaga_real = 0;
    pcb->estimacion_rafaga = estimacion_rafaga;
    pcb->estimacion_rafaga_anterior = estimacion_rafaga;
    pcb->estado = NEW;
    
    return pcb;
}

t_pcb* armar_PCB(int id_proceso, int tamanio, t_list* instrucciones, double estimacion_rafaga, int pc, 
                char estado,double ultima_rafaga,double ultima_rafaga_real,int tabla_paginas) {
    t_pcb* pcb = crear_PCB(id_proceso, tamanio, instrucciones, estimacion_rafaga);
    pcb->tabla_paginas = tabla_paginas;
    pcb->ultima_rafaga = ultima_rafaga;
    pcb->ultima_rafaga_real = ultima_rafaga_real;
    pcb->pc = pc;
    pcb->estimacion_rafaga_anterior = estimacion_rafaga;
    pcb->estado = estado;

    return pcb;
}


void cambiar_estado(t_pcb* pcb, char estado){
    pcb->estado = estado;
}

// Revisar
void eliminar_PCB(void* pcb) {
   // memcpy(t_pcb* pcbAux,(t_pcb*) pcb,size(pcb));
    t_pcb* pcb_aux = (t_pcb*) pcb;
    list_destroy_and_destroy_elements(pcb_aux->instrucciones, element_destroyer); 
    free(pcb_aux);
    //free(pcb);
}

int size_PCB(t_pcb* pcb) {
	return sizeof(int)*4 + list_size(pcb->instrucciones);
}

t_paquete* armar_paquete_pcb(t_pcb* pcb) {
    t_paquete* paquete = crear_paquete();
    agregar_a_paquete_pcb (paquete, pcb);
    return paquete;	
}

t_pcb* recepcionar_paquete_pcb(int socket) {
    t_list* lista = recibir_paquete(socket);
    t_list_iterator* iterador = list_iterator_create(lista);

    int* id_proceso          = (int*) list_iterator_next(iterador);
    int* tamanio            = (int*) list_iterator_next(iterador);
    int* tabla_paginas            = (int*) list_iterator_next(iterador);
    double* ultima_rafaga    = (double*) list_iterator_next(iterador);
    double* ultima_rafaga_real = (double*) list_iterator_next(iterador);
    double* estimacion_rafaga   = (double*) list_iterator_next(iterador);
    double* estimacion_rafaga_anterior = (double*) list_iterator_next(iterador);
    int* pc                 = (int*) list_iterator_next(iterador);
    char* estado            = (char*) list_iterator_next(iterador);
    
    t_list* instrucciones = list_create();
    while(list_iterator_has_next(iterador)) {
        t_instruccion* instruccion = list_iterator_next(iterador);
        list_add(instrucciones, instruccion);
    }

    list_iterator_destroy(iterador);

    t_pcb* pcb = armar_PCB(*id_proceso, *tamanio, instrucciones, *estimacion_rafaga, *pc, *estado,*ultima_rafaga,*ultima_rafaga_real,*tabla_paginas);

    list_destroy(lista);
    free(id_proceso);
    free(tamanio);
    free(ultima_rafaga);
    free(ultima_rafaga_real);
    free(estimacion_rafaga);
    free(estimacion_rafaga_anterior);
    free(pc);
    free(estado);
    free(tabla_paginas);
    return pcb;
}

void agregar_a_paquete_pcb(t_paquete* paquete, t_pcb* pcb) {
    agregar_a_paquete(paquete,&(pcb->id_proceso), sizeof(int));
    agregar_a_paquete(paquete,&(pcb->tamanio), sizeof(int));
    agregar_a_paquete(paquete,&(pcb->tabla_paginas), sizeof(int));
    agregar_a_paquete(paquete,&(pcb->ultima_rafaga), sizeof(double));
    agregar_a_paquete(paquete,&(pcb->ultima_rafaga_real), sizeof(double));
    agregar_a_paquete(paquete,&(pcb->estimacion_rafaga), sizeof(double));
    agregar_a_paquete(paquete,&(pcb->estimacion_rafaga_anterior),sizeof(double));
    agregar_a_paquete(paquete,&(pcb->pc), sizeof(int));
    agregar_a_paquete(paquete,&(pcb->estado), sizeof(char));

    t_list_iterator* iterador = list_iterator_create(pcb->instrucciones);
    while(list_iterator_has_next(iterador)) {
        t_instruccion* instruccion = (t_instruccion*) list_iterator_next(iterador);
        agregar_a_paquete(paquete, instruccion, 9);   
    }

	list_iterator_destroy(iterador); 
}





void recepcionar_paquete_IO(int socket, int** tiempo_bloqueo, t_pcb** pcb) {
    t_list* lista = recibir_paquete(socket);
    t_list_iterator* iterador = list_iterator_create(lista);

    // Tiempo bloqueo
    *tiempo_bloqueo             = (int*) list_iterator_next(iterador);

    int* id_proceso             = (int*) list_iterator_next(iterador);
    int* tamanio                = (int*) list_iterator_next(iterador);
    int* tabla_paginas         = (int*) list_iterator_next(iterador);
    double* ultima_rafaga       = (double*) list_iterator_next(iterador);
    double* ultima_rafaga_real  = (double*) list_iterator_next(iterador);
    double* estimacion_rafaga   = (double*) list_iterator_next(iterador);
    double* estimacion_rafaga_anterior = (double*) list_iterator_next(iterador);
    int* pc                     = (int*) list_iterator_next(iterador);
    char* estado                = (char*) list_iterator_next(iterador);
    
    t_list* instrucciones = list_create();
    while(list_iterator_has_next(iterador)) {
        t_instruccion* instruccion = list_iterator_next(iterador);
        list_add(instrucciones, instruccion);
    }

    list_iterator_destroy(iterador);

    *pcb = armar_PCB(*id_proceso, *tamanio, instrucciones, *estimacion_rafaga, *pc, *estado,*ultima_rafaga,*ultima_rafaga_real,*tabla_paginas);

    free(id_proceso);
    free(tamanio);
    free(ultima_rafaga);
    free(ultima_rafaga_real);
    free(estimacion_rafaga);
    free(estimacion_rafaga_anterior);
    free(pc);
    free(estado);
    free(tabla_paginas);
}
