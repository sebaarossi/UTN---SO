#include "planificador_corto_plazo.h"

void planificador_corto_plazo(){
   t_pcb* pcb;

    while(1){
    // Podes entrar por 4 alternativas: 
    // - Te llama el de largo plazo por un nuevo proceso. 
    // - Te avisa mediano plazo que hay uno en blocked y puede entrar a ready.
    // - Te avisa mediano plazo que uno de suspended ready puede entrar a ready.
    // - Te avisa el de largo plazo que termino un proceso.   
        
            switch(algoritmo){
                case FIFO:  
                            sem_wait(&sem_procesos_en_ready);
                            sem_wait(&sem_corto_plazo_FIFO);
                            
                            pthread_mutex_lock(&mutex_ready);
                            pcb = list_remove(cola_ready,0);
                            pthread_mutex_unlock(&mutex_ready);

                            log_info(logger,"Proceso con id %d: Ready -> Exec",pcb->id_proceso);

                            t_paquete* paquete_cpu_FIFO = armar_paquete_pcb(pcb);
                            enviar_paquete(paquete_cpu_FIFO,socket_cpu_dispatch);
                        
                            eliminar_PCB((void*) pcb);
                            eliminar_paquete(paquete_cpu_FIFO);
                            break;

                case SRT: // SRT
                            sem_wait(&sem_corto_plazo);
                            
                            if(list_size(cola_ready) > 0){
                                
                            //MUTEX flag_exec
                             pthread_mutex_lock(&mutex_flag_exec);
                                if(flag_exec==1){
                                    pthread_mutex_unlock(&mutex_flag_exec);
                                    //sem_wait(&recibi_interrupt);
                                    enviar_mensaje("Desaloja",socket_cpu_interrupt);    
                                   
                                }
                                else{  
                                    pthread_mutex_unlock(&mutex_flag_exec); 
                                    
                                    pthread_mutex_lock(&mutex_ready);
                                    pcb = obtener_proceso_con_menor_estimacion();
                                    pthread_mutex_unlock(&mutex_ready);
                                    
                                    log_info(logger,"Proceso con id %d: Ready -> Exec",pcb->id_proceso);
                                    enviar_paquete_pcb(pcb);
                                    pthread_mutex_lock(&mutex_flag_exec);
                                    flag_exec = 1;
                                    pthread_mutex_unlock(&mutex_flag_exec);
                                }
                            }
                            

                            break;
                            
            } 
           
    }
}



//////////////////////////////////////////////////////////
t_pcb* obtener_proceso_con_menor_estimacion(){
    t_pcb* pcb_a_comparar = list_get(cola_ready,0);
    t_pcb* pcb;
    int indice =0;
    int i =-1;
    if(list_size(cola_ready)>1){
        t_list_iterator* iterator = list_iterator_create(cola_ready);
    
        while(list_iterator_has_next(iterator) ){
            pcb = list_iterator_next(iterator);
            i++;
            if(pcb_a_comparar->estimacion_rafaga > pcb->estimacion_rafaga){
                pcb_a_comparar = pcb;
                indice = i;
            }
            
        }
        list_iterator_destroy(iterator);
    }
    
    list_remove(cola_ready,indice);

    return pcb_a_comparar;
}

void enviar_paquete_pcb(t_pcb* pcb) {
    cambiar_estado(pcb,EXEC);
    t_paquete* paquete_cpu_SRT = armar_paquete_pcb(pcb);
    enviar_paquete(paquete_cpu_SRT,socket_cpu_dispatch);
    eliminar_paquete(paquete_cpu_SRT);
}
