#include "planificador_mediano_plazo.h"

void planificador_mediano_plazo(){   
    pthread_t thread_controlador_suspension;
    
    while(1){
        sem_wait(&sem_mediano_plazo);

        pthread_mutex_lock(&mutex_blocked);
        t_pcb *pcb = list_get(cola_blocked,list_size(cola_blocked)-1);

        pthread_mutex_unlock(&mutex_blocked);
        
        pthread_create(&thread_controlador_suspension,NULL,(void*) controlar_suspension,&(pcb->id_proceso));
        pthread_detach(thread_controlador_suspension);
        
        sem_post(&sem_io);
    }
}


t_suspension* buscar_suspension(int id){
    
    bool buscar_en_suspension(t_suspension* suspension){
        return suspension->id_proceso == id; 
    }
    t_suspension* susp = list_find(estado_suspendidos,(void*) buscar_en_suspension);
    return susp;
}

void controlar_suspension(int* id){

    int tiempo_maximo = get_tiempo_maximo_bloqueado();
    int contador_suspension_viejo;
    t_pcb* pcb;

    //mutex
    t_suspension* susp = buscar_suspension(*id);
    //mutex
    contador_suspension_viejo = susp->contador;
    usleep(tiempo_maximo * 1000);
    pthread_mutex_lock(&mutex_blocked);
    pcb = estaBloqueado(*id);


    if(pcb != NULL && susp->contador == contador_suspension_viejo){

        cambiar_estado(pcb,SUSPENDED_BLOCKED);
        pthread_mutex_unlock(&mutex_blocked);
            t_paquete* paquete = crear_paquete_con_codigo_especial(SUS_PRO);
            agregar_a_paquete(paquete,&(pcb->id_proceso),sizeof(int));
            log_info(logger,"Proceso con id %d: Enviado a suspension",pcb->id_proceso);
            enviar_paquete(paquete,socket_memoria);
            eliminar_paquete(paquete);
            recibir_operacion(socket_memoria);
            recibir_mensaje(socket_memoria,logger);
            
            log_info(logger,"Proceso con id %d: Blocked -> Suspended-Blocked",pcb->id_proceso);
            sem_post(&espera_suspension);
        sem_post(&sem_multiprogramacion);
    }
    else{
        pthread_mutex_unlock(&mutex_blocked);
    }
     
}

t_pcb* estaBloqueado(int id_proceso){

    t_list_iterator* iterator = list_iterator_create(cola_blocked);
    t_pcb* pcb;
    while(list_iterator_has_next(iterator)){
        pcb = list_iterator_next(iterator);
        if(id_proceso == pcb->id_proceso)
            return pcb;
    }
    return NULL;
}
    
void ejecucion_IO(){

    t_pcb* pcb;
    int* tiempo_bloqueo;
   
    while(1){
        sem_wait(&sem_io);

        pthread_mutex_lock(&mutex_blocked);
        pcb = list_get(cola_blocked,0);
        tiempo_bloqueo = list_remove(cola_suspended_times,0);
        pthread_mutex_unlock(&mutex_blocked);
        
        usleep(*tiempo_bloqueo * 1000);
        
        pthread_mutex_lock(&mutex_blocked);
        if(pcb->estado == SUSPENDED_BLOCKED ){
            
            
            pcb = list_remove(cola_blocked,0);
            pthread_mutex_unlock(&mutex_blocked);

            cambiar_estado(pcb,SUSPENDED_READY);

            pthread_mutex_lock(&mutex_suspended_ready);
            list_add(cola_suspended_ready,pcb);
            pthread_mutex_unlock(&mutex_suspended_ready);

            sem_wait(&espera_suspension);
            log_info(logger,"Proceso con id %d: Suspended-Blocked -> Suspended-Ready",pcb->id_proceso);


            sem_post(&sem_largo_plazo);

            //cargar en memoria.

        }
        else{
           
            pcb = list_remove(cola_blocked,0);
            pthread_mutex_unlock(&mutex_blocked);
            
            bool buscar_en_suspension(t_suspension* suspension){
                return suspension->id_proceso == pcb->id_proceso; 
            }
            t_suspension* susp = list_find(estado_suspendidos,(void*) buscar_en_suspension);
            susp->contador++;

            pthread_mutex_lock(&mutex_ready);
            cambiar_estado(pcb,READY);
            list_add(cola_ready,pcb);
            pthread_mutex_unlock(&mutex_ready);
            
            log_info(logger,"Proceso con id %d: Blocked -> Ready",pcb->id_proceso);

            sem_post(&sem_procesos_en_ready);
            sem_post(&sem_corto_plazo);
            //sem_post(&sem_corto_plazo_FIFO);
        }

        
    }
}  