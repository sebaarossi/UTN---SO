#ifndef TLB_H
#define TLB_H

#include "configCpu.h"
#include "cpu_init.h"

#define LIBRE   0
#define OCUPADO 1

typedef struct tlbEntry {
    uint32_t pid;
    uint32_t pagina;
    int32_t marco;
    int isFree;
} t_tlbEntry;


typedef struct tlb {
    t_tlbEntry* entradas;
    unsigned int tamanio;
    t_list* victim_queue;
} t_tlb;


t_tlb* tlb;
pthread_mutex_t mutex_tlb;
int algoritmo_tlb;

t_tlb* createTLB();
void agregar_entrada_tlb(uint32_t pid, uint32_t pagina, int32_t marco);
int32_t obtener_marco(uint32_t pid, uint32_t pagina);
void actualizar_victim_queue(t_tlbEntry* entrada);
void agregar_entrada_tlb(uint32_t pid, uint32_t pagina, int32_t marco);
void algoritmo_lru(t_tlbEntry* entrada);
void algoritmo_fifo(t_tlbEntry* entrada);
void quitar_entrada(uint32_t pid, uint32_t pagina);
void destruir_tlb(t_tlb* tlb);
void limpiar_tlb();
void liberar_entradas_de_un_proceso(uint32_t pid);

#endif