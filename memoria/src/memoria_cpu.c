#include "memoria_utils.h"
#include "memoria_cpu.h"

void* recepcionar_cpu(){
    socket_cliente_cpu = esperar_cliente(socket_memoria_server);
    sem_post(&inicio_cpu);

    while(1){
        int op_code = recibir_operacion(socket_cliente_cpu);
        
        switch (op_code){
        
        case INI_CPU:
            pedido_ini_cpu(socket_cliente_cpu);
            break;
        
        case FET_OP:
            pedido_fet_op(socket_cliente_cpu);
            break;

        case PED_LEC:
            pedido_de_lectura();
            break;
        
        case PED_ESC:
            pedido_de_escritura();
            break;
        
        case PED_COPY:
            pedido_de_copy(); 
            break;

        case ACC_TAB_1:
            acceder_tp_1();
            break;
        
        case ACC_TAB_2:
            acceder_tp_2();
            break;
        }
    }
}

void pedido_ini_cpu(int socket) {
    t_paquete* paquete_CPU = crear_paquete();

    agregar_a_paquete(paquete_CPU, &tam_pagina,sizeof(int));
    agregar_a_paquete(paquete_CPU, &entradas_por_tabla,sizeof(int));

    enviar_paquete(paquete_CPU,socket);

    eliminar_paquete(paquete_CPU);
}

void pedido_fet_op(){
    t_list* lista= recibir_paquete(socket_cliente_cpu);
    uint32_t* direc_fisica = list_remove(lista,0);

    uint32_t valor = buscar_valor_memoria(*direc_fisica);
    log_info(logger,"Fetch operands: Se leyo el valor %d de la posicion %d",valor,*direc_fisica);
    t_paquete* paquete_CPU = crear_paquete();
    agregar_a_paquete(paquete_CPU, &valor,sizeof(uint32_t));

    enviar_paquete(paquete_CPU,socket_cliente_cpu);
    
    eliminar_paquete(paquete_CPU);
}

void pedido_de_lectura(void) {
    t_list* lista= recibir_paquete(socket_cliente_cpu);
    uint32_t* direc_fisica = list_remove(lista,0);
 
    uint32_t valor = buscar_valor_memoria(*direc_fisica);
    log_info(logger,"Read: Valor leido: %d",valor);
    t_paquete* paquete = crear_paquete();
    agregar_a_paquete(paquete,&valor,sizeof(uint32_t));

    enviar_paquete(paquete,socket_cliente_cpu);

    eliminar_paquete(paquete);
    free(direc_fisica);
    list_destroy(lista);
}

void pedido_de_escritura(void) {
    t_list* lista= recibir_paquete(socket_cliente_cpu);
    uint32_t* direc_fisica = list_remove(lista,0);
    uint32_t* valor = list_remove(lista,0);
    
    escribir_valor_memoria(*direc_fisica, *valor);
    
    log_info(logger,"Write: Se escribio el valor %d en la posicion de memoria %d",*valor,*direc_fisica);

    enviar_mensaje("OK", socket_cliente_cpu);

    free(direc_fisica);
    free(valor);
}

void pedido_de_copy(void) {
    t_list* lista= recibir_paquete(socket_cliente_cpu);
    uint32_t* direc_fisica_dest =  list_remove(lista,0); 
    uint32_t* valor_copy =  list_remove(lista,0);

    escribir_valor_memoria(*direc_fisica_dest, *valor_copy);
    log_info(logger,"Copy: Se escribio el valor %d en la posicion de memoria %d",*valor_copy,*direc_fisica_dest);

    //enviar_mensaje("Copy ok", socket_cliente_cpu);
    free(direc_fisica_dest);
    free(valor_copy);
}

uint32_t buscar_valor_memoria(uint32_t direc_fisica) {
    uint32_t valor_memoria;

    memcpy(&valor_memoria,memoria_ram + direc_fisica,sizeof(uint32_t));
    int marco_leido = floorl(direc_fisica / tam_pagina); 
    
    t_entrada_segundo_nivel* entrada = buscar_entrada(marco_leido);    

    entrada->bit_de_uso = 1;
    return valor_memoria;
}


t_entrada_segundo_nivel* buscar_entrada(int marco_a_buscar) {
    t_entrada_segundo_nivel* entrada_a_modificar = NULL;
    
    t_pagina_primer_nivel* tabla_primer_nivel = list_get(tabla_paginas_primer_nivel,tabla_primer_nivel_global);

    bool es_la_tabla(t_pagina_segundo_nivel* tabla ){
        return tabla->id_proceso == tabla_primer_nivel->id_proceso;
    }   
    t_list* tablas_segundo = list_filter(tabla_paginas_segundo_nivel,(void*) es_la_tabla);
   
    t_list* entradas_segundo_nivel = list_create();

    void agrego_entradas(t_list* lista_de_entradas, t_pagina_segundo_nivel* una_tabla){
        list_add_all(entradas_segundo_nivel,una_tabla->entradas_segundo_nivel);
    }
    list_fold(tablas_segundo,entradas_segundo_nivel,(void*) agrego_entradas);


    bool es_la_entrada(t_entrada_segundo_nivel* entrada){
        return entrada->marco == marco_a_buscar && entrada->bit_presencia==1;
    }
    
    entrada_a_modificar = list_find(entradas_segundo_nivel,(void*) es_la_entrada);

    
    list_destroy(tablas_segundo);
    list_destroy(entradas_segundo_nivel);
    
    return entrada_a_modificar;
}


void escribir_valor_memoria(uint32_t direc_fisica_dest, uint32_t valor) {
    memcpy(memoria_ram + direc_fisica_dest, &valor, sizeof(uint32_t));

    int marco_modificado = floorl(direc_fisica_dest / tam_pagina);
    
    t_entrada_segundo_nivel* entrada = buscar_entrada(marco_modificado);    
    entrada->bit_de_modificado = true;
    entrada->bit_de_uso = true;
}


void acceder_tp_1(void){
    t_list* paquete_cpu = recibir_paquete(socket_cliente_cpu);

    int* tabla_pagina_1er_nivel = list_remove(paquete_cpu, 0);
    int* entrada =  list_remove(paquete_cpu, 0);

    list_destroy(paquete_cpu);

    t_pagina_primer_nivel* tabla_1er_nivel = list_get(tabla_paginas_primer_nivel, *tabla_pagina_1er_nivel);
    int* entrada_segundo_nivel = (int*) list_get(tabla_1er_nivel->entradas_primer_nivel,*entrada);
    tabla_primer_nivel_global = *tabla_pagina_1er_nivel;

    t_paquete* paquete = crear_paquete();
    agregar_a_paquete(paquete, entrada_segundo_nivel, sizeof(int));

    enviar_paquete(paquete, socket_cliente_cpu);

    eliminar_paquete(paquete);
}


void acceder_tp_2(void) {
    t_list* paquete_cpu = recibir_paquete(socket_cliente_cpu);

    int marco_a_reemplazar;
    int* tabla_pagina_2do_nivel = list_remove(paquete_cpu, 0);
    int* entrada =  list_remove(paquete_cpu, 0);
    uint32_t * pagina = (uint32_t*) list_remove(paquete_cpu, 0);

    list_destroy(paquete_cpu);

    t_pagina_segundo_nivel* tabla_2do_nivel = list_get(tabla_paginas_segundo_nivel, *tabla_pagina_2do_nivel);
    t_entrada_segundo_nivel* entrada_segundo_nivel = list_get(tabla_2do_nivel->entradas_segundo_nivel, *entrada);

    int id_proceso = tabla_2do_nivel -> id_proceso;

    if(entrada_segundo_nivel->bit_presencia == 0){
        sem_wait(&sem_swap);
        void* contenido_pag= leer_swap(id_proceso,*pagina);
        sem_post(&sem_swap);

        if( string_equals_ignore_case(algoritmo,CLOCK))
            marco_a_reemplazar= algoritmo_clock(id_proceso);
        else 
            marco_a_reemplazar= algoritmo_clock_modificado(id_proceso);
            
        uint32_t direccion_fisica = marco_a_reemplazar * tam_pagina;
        escribir_dato_memoria(direccion_fisica, contenido_pag);

        log_info(logger,"Marco %d asignado a pagina %d en direccion logica %d",marco_a_reemplazar,*pagina,*pagina * tam_pagina);
        entrada_segundo_nivel->marco = marco_a_reemplazar;
        entrada_segundo_nivel->bit_presencia = 1;
        if(primer_marco_asignado(tabla_2do_nivel->id_proceso) == 1) {
            entrada_segundo_nivel->indice = 1;
        }

    }

    // se fija si esta en memoria ... si no esta la carga
    t_paquete* paquete = crear_paquete();
    agregar_a_paquete(paquete, &(entrada_segundo_nivel->marco), sizeof(int));
    enviar_paquete(paquete, socket_cliente_cpu);
  
    eliminar_paquete(paquete);
    free(entrada);
    free(pagina);
}

void escribir_dato_memoria(uint32_t direc_fisica_dest, void * valor) {
    memcpy(memoria_ram + direc_fisica_dest, valor, tam_pagina);
}

int primer_marco_asignado(int id_proceso) {
	int i=0, cantidad=0;

	for(i=0 ; i<cantidad_de_marcos_totales ; i++) {
		if(bitarray_marcos[i] == id_proceso)
			cantidad++;
	}

	return cantidad;
}
