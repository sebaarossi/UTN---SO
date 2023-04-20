#include "planificador_largo_plazo.h"


void planificador_largo_plazo() { 
    t_pcb* pcb;
   
    while(1){        
        sem_wait(&sem_largo_plazo);
		sem_wait(&sem_multiprogramacion);
        
        if(list_size(cola_suspended_ready) > 0){
            pthread_mutex_lock(&mutex_suspended_ready);
            pcb = list_remove(cola_suspended_ready,0);
            pthread_mutex_unlock(&mutex_suspended_ready);

            cambiar_estado(pcb,READY);

            pthread_mutex_lock(&mutex_ready);
            list_add(cola_ready,pcb);
            pthread_mutex_unlock(&mutex_ready);

            log_info(logger,"Proceso con id %d: Suspended-Ready -> Ready",pcb->id_proceso);
           
        }
        else{
            pthread_mutex_lock(&mutex_new);
            pcb = list_remove(cola_new,0);
            pthread_mutex_unlock(&mutex_new);

            t_paquete* paquete = crear_paquete_con_codigo_especial(INI_PRO);
            agregar_a_paquete(paquete,&(pcb->id_proceso),sizeof(int));
            agregar_a_paquete(paquete,&(pcb->tamanio),sizeof(int));
            enviar_paquete(paquete,socket_memoria);
            eliminar_paquete(paquete);
            
            recibir_operacion(socket_memoria);
            t_list* lista = recibir_paquete(socket_memoria);
            int* tabla = list_remove(lista,0);
            pcb->tabla_paginas = *tabla;
        
            list_destroy(lista);

            cambiar_estado(pcb,READY);
            
            pthread_mutex_lock(&mutex_ready);
            list_add(cola_ready,pcb);
            pthread_mutex_unlock(&mutex_ready);

            log_info(logger,"Proceso con id %d: New -> Ready ",pcb->id_proceso);
        }
        
        sem_post(&sem_procesos_en_ready);
        sem_post(&sem_corto_plazo);
    }
}
