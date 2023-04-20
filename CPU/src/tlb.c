#include "tlb.h"


t_tlb* createTLB() {
    //Inicializo estructura de TLB
    tlb = malloc(sizeof(t_tlb));
    tlb->tamanio = get_entradas_tlb();
    tlb->entradas = (t_tlbEntry*) calloc(tlb->tamanio, sizeof(t_tlbEntry));
    tlb->victim_queue = list_create();
    pthread_mutex_init(&mutex_tlb, NULL);

    //Seteo todas las entradas como libres
    for(int i = 0; i < tlb->tamanio; i++) {
        tlb->entradas[i].isFree = LIBRE;
    }

    algoritmo_tlb = get_algoritmo_tlb();

    return tlb;
}


int32_t obtener_marco(uint32_t pid, uint32_t pagina) {
    int32_t marco = -1;

    pthread_mutex_lock(&mutex_tlb);
    for(int i = 0; i < tlb->tamanio; i++) {
        if(tlb->entradas[i].pid == pid && tlb->entradas[i].pagina == pagina && tlb->entradas[i].isFree == OCUPADO) {
            actualizar_victim_queue(&(tlb->entradas[i]));
            marco = tlb->entradas[i].marco;
        }
    }
    pthread_mutex_unlock(&mutex_tlb);

    if(marco == -1) 
        log_info(logger, "TLB Miss: Proceso %d, Pagina %d", pid, pagina);
    else 
        log_info(logger, "TLB Hit: Proceso %d, Pagina %d, marco %d", pid, pagina, marco);

    return marco;
}

void actualizar_victim_queue(t_tlbEntry* entrada) {
    switch(algoritmo_tlb){
        
        case LRU: 
                    algoritmo_lru(entrada);
                    break;
        case FIFO:
                    algoritmo_fifo(entrada);
    }
}

void agregar_entrada_tlb(uint32_t pid, uint32_t pagina, int32_t marco) {
    //Busco si hay una entrada libre
    int encontro_libre = 0;

    pthread_mutex_lock(&mutex_tlb);
    for(int i = 0 ; i < tlb->tamanio && encontro_libre == 0 ; i++) {
        if(tlb->entradas[i].isFree == LIBRE) {
            tlb->entradas[i].pid = pid;
            tlb->entradas[i].pagina = pagina;
            tlb->entradas[i].marco = marco;
            tlb->entradas[i].isFree = OCUPADO;

            encontro_libre = 1;
            list_add(tlb->victim_queue, &(tlb->entradas[i]));
            break;
        }
    }

    //Si no hay entrada libre, reemplazo
    if(encontro_libre == 0) {
        if(!list_is_empty(tlb->victim_queue)){
            t_tlbEntry* victima = list_remove(tlb->victim_queue, 0);

            log_info(logger, "ALGORITMO REMPLAZO TLB, \n\tSale pagina: %d, marco: %d \n\tEntra Pagina: %d, marco: %d", victima->pagina, victima->marco, pagina, marco);

            victima->pid = pid;
            victima->pagina = pagina;
            victima->marco = marco;
            victima->isFree = OCUPADO;

            list_add(tlb->victim_queue, victima);
        }
    }

    pthread_mutex_unlock(&mutex_tlb);
}

void algoritmo_fifo(t_tlbEntry* entrada) {

}


void algoritmo_lru(t_tlbEntry* entrada) {
    bool es_victima(t_tlbEntry* victima) {
        return victima == entrada;
    }

    t_tlbEntry* entrada_removida = list_remove_by_condition(tlb->victim_queue, (void*) es_victima);

    list_add(tlb->victim_queue, entrada_removida);
}


void quitar_entrada(uint32_t pid, uint32_t pagina) {
    pthread_mutex_lock(&mutex_tlb);
    for(int i = 0; i < tlb->tamanio; i++) {
        if(tlb->entradas[i].pid == pid && tlb->entradas[i].pagina == pagina) {
            tlb->entradas[i].isFree = LIBRE;
            pthread_mutex_unlock(&mutex_tlb);
            return;
        }
    }
    pthread_mutex_unlock(&mutex_tlb);
}

void destruir_tlb(t_tlb* tlb) {
    free(tlb->entradas);
    list_destroy(tlb->victim_queue);
    pthread_mutex_destroy(&mutex_tlb);
    free(tlb);
}

void limpiar_tlb() {
    pthread_mutex_lock(&mutex_tlb);
    for(int i = 0; i < tlb->tamanio; i++) {
        tlb->entradas[i].isFree = LIBRE;
    }
    pthread_mutex_unlock(&mutex_tlb);
}

// Libera las entradas de un proceso específico
void liberar_entradas_de_un_proceso(uint32_t pid) {
    pthread_mutex_lock(&mutex_tlb);
    for(int i = 0; i < tlb->tamanio; i++) {
        if(tlb->entradas[i].pid == pid) {
            tlb->entradas[i].isFree = LIBRE;
        }
    }
    pthread_mutex_unlock(&mutex_tlb);
}
