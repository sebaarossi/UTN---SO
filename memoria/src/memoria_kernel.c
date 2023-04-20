#include "memoria_kernel.h"
#include "string.h"
#include "algoritmos_swap.h"


void* recepcionar_kernel(){
    sem_wait(&inicio_cpu);
    socket_cliente_kernel = esperar_cliente(socket_memoria_server);

    while(1){
        int op_code = recibir_operacion(socket_cliente_kernel);
        
        switch(op_code){
        case INI_PRO: 
                inicializar_proceso();
                break;
        
        case SUS_PRO:
            suspender_proceso();
            enviar_mensaje("Proceso suspendido",socket_cliente_kernel);
            break;
        
        case FIN_PRO:
            finalizar_proceso();
            break;
    }      
    }
}

void inicializar_proceso(){
    t_list* lista = recibir_paquete(socket_cliente_kernel);
    int* id_proceso = list_remove(lista,0);
    int* tamanio_proceso = list_remove(lista,0);
    list_destroy(lista);

    int posicion_tabla_paginas_primer_nivel = crear_estructuras(*id_proceso,*tamanio_proceso);

    crear_archivo_swap(*id_proceso,*tamanio_proceso);
    log_info(logger,"Proceso %d inicializado con tamanio %d Bytes",*id_proceso,*tamanio_proceso);
    t_paquete* paquete_tp = crear_paquete();
    agregar_a_paquete(paquete_tp,&posicion_tabla_paginas_primer_nivel,sizeof(int));
    enviar_paquete(paquete_tp,socket_cliente_kernel);

    eliminar_paquete(paquete_tp);
    free(id_proceso);
    free(tamanio_proceso);
}

int crear_estructuras(int id_proceso,int tamanio_proceso) {
    int num_pagina = 0;
    int i = 0;
    pthread_mutex_lock(&mutex_tabla_paginas);
    t_pagina_primer_nivel* tabla_primer_nivel = malloc(sizeof(t_pagina_primer_nivel));
    tabla_primer_nivel->entradas_primer_nivel = list_create();
    tabla_primer_nivel->id_proceso = id_proceso;

    int posicion_1er_nivel = agregar_tabla_primer_nivel(tabla_primer_nivel);
    
    double tamanio = (tamanio_proceso / (float) tam_pagina) * 1.0;
    int cant_paginas_de_proceso = ceil(tamanio);
    
    for(i=0; i<ceil(cant_paginas_de_proceso/entradas_por_tabla);i++) {
        int j = 0;
        t_pagina_segundo_nivel* tabla_segundo_nivel = malloc(sizeof(t_pagina_segundo_nivel));
        tabla_segundo_nivel->id_proceso = id_proceso;
        tabla_segundo_nivel->entradas_segundo_nivel = list_create();
        for(j=0;j<entradas_por_tabla;j++){
            t_entrada_segundo_nivel* entrada = malloc(sizeof(t_entrada_segundo_nivel));
            entrada->pagina = num_pagina;
            entrada->marco = -1;
            entrada->bit_presencia = 0;
            entrada->bit_de_uso = false;
            entrada->bit_de_modificado = false;
            list_add(tabla_segundo_nivel->entradas_segundo_nivel,entrada);
            num_pagina++;
        }

        // Agrego a la segunda tabla de paginas general
        int* posicion_2do_nivel = malloc(sizeof(int));
        *posicion_2do_nivel  = agregar_tabla_segundo_nivel(tabla_segundo_nivel);

        // Agrego a la primer tabla de paginas general
        list_add(tabla_primer_nivel->entradas_primer_nivel,posicion_2do_nivel);
    }
    pthread_mutex_unlock(&mutex_tabla_paginas);

    return posicion_1er_nivel;
}


void suspender_proceso() {
    // Liberar memoria
    t_list* lista = recibir_paquete(socket_cliente_kernel);
    int* id_proceso = list_remove(lista,0);
    
    log_info(logger, "Me pidieron suspender el proceso: %d", *id_proceso);
    list_destroy(lista);

    t_list* paginas_modificadas= obtener_entradas_paginas_modificadas(tabla_primer_nivel_global, *id_proceso);
    t_entrada_segundo_nivel* ent_2do_nivel;

    for(int i=0 ; i < list_size(paginas_modificadas); i++ ) {
        ent_2do_nivel = list_get(paginas_modificadas, i);
        
    
        void* contenido_pag = leer_dato_memoria(ent_2do_nivel -> marco);
	    int pagina = ent_2do_nivel -> pagina;

        sem_wait(&sem_swap);
        escribir_en_swap(*id_proceso, contenido_pag, pagina);
        sem_post(&sem_swap);
        ent_2do_nivel->marco = -1;
        ent_2do_nivel->bit_presencia = false;
        ent_2do_nivel->bit_de_modificado=false;
        ent_2do_nivel->bit_de_uso = false;
    }

    liberar_marcos(*id_proceso);
    
    free(id_proceso);
}

t_list* obtener_entradas_paginas_modificadas(int tabla_primer_nivel_global, int proceso_id) {
    t_pagina_primer_nivel* tabla_primer_nivel = list_get(tabla_paginas_primer_nivel,tabla_primer_nivel_global);
	/*t_list* lista_auxiliar = list_create();
    int* entrada;
    t_pagina_segundo_nivel* tabla_segundo_nivel;
    t_entrada_segundo_nivel* ent_2do_nivel;*/
    // Armo tabla aux de entradas de segundo nivel
    
    t_list* marcos_modificados = list_create();

    void agrego_entradas(t_list* lista_de_entradas,int* entrada){
        t_pagina_segundo_nivel* tabla = list_get(tabla_paginas_segundo_nivel,*entrada);

        list_add_all(marcos_modificados,tabla->entradas_segundo_nivel);
    }
    list_fold(tabla_primer_nivel->entradas_primer_nivel,marcos_modificados,(void*) agrego_entradas);

    bool esta_modificado(t_entrada_segundo_nivel* entrada){
        return entrada->bit_presencia == 1 && entrada->bit_de_modificado == 1;
    }
    marcos_modificados = list_filter(marcos_modificados,(void*) esta_modificado);
   

   
    /*    
    for(int i=0; i < list_size(tabla_primer_nivel->entradas_primer_nivel);i++){

        entrada = list_get(tabla_primer_nivel->entradas_primer_nivel,i);

        tabla_segundo_nivel = list_get(tabla_paginas_segundo_nivel, *entrada);

        if(tabla_segundo_nivel -> id_proceso == proceso_id){
            t_list_iterator* iterator = list_iterator_create(tabla_segundo_nivel->entradas_segundo_nivel);

		    while(list_iterator_has_next(iterator)){
			    ent_2do_nivel = list_iterator_next(iterator);
		
			    if(ent_2do_nivel->bit_presencia == 1 && ent_2do_nivel->bit_de_modificado == 1){
				    list_add(lista_auxiliar,ent_2do_nivel);
			    }
		    }
        }
        
    }

    */
	return marcos_modificados;
}




void finalizar_proceso() {
    t_list* lista = recibir_paquete(socket_cliente_kernel);
    int* id_proceso = list_remove(lista,0);

    liberar_marcos(*id_proceso);
    
    log_info(logger,"Proceso %d finalizado",*id_proceso);

    list_destroy(lista);
    borrar_archivo_swap(*id_proceso);
    free(id_proceso);
}



int agregar_tabla_primer_nivel(t_pagina_primer_nivel* tabla_primer_nivel) {
    //t_list_iterator* iterator = list_iterator_create(tabla_paginas_primer_nivel);
    int i;

    i = list_size(tabla_paginas_primer_nivel) ;

    list_add_in_index(tabla_paginas_primer_nivel,i,tabla_primer_nivel);
/*
    while(list_iterator_has_next(iterator)) {
        t_pagina_primer_nivel* pagina_1er_nivel = list_iterator_next(iterator);
        
        if(pagina_1er_nivel == NULL) {
            list_replace(tabla_paginas_primer_nivel, i, tabla_primer_nivel);
            break;
        }

        i++;
    }
    list_iterator_destroy(iterator);

    if(i == list_size(tabla_paginas_primer_nivel)) {
        list_add(tabla_paginas_primer_nivel, tabla_primer_nivel);
        i = list_size(tabla_paginas_primer_nivel) - 1;
    }
*/
    return i;    
}


int agregar_tabla_segundo_nivel(t_pagina_segundo_nivel* tabla_segundo_nivel) {
    
    //t_list_iterator* iterator = list_iterator_create(tabla_paginas_segundo_nivel);
    int i=0;

     i = list_size(tabla_paginas_segundo_nivel)  ;

    list_add_in_index(tabla_paginas_segundo_nivel,i,tabla_segundo_nivel);
/*
    while(list_iterator_has_next(iterator)) {
        t_pagina_segundo_nivel* pagina_2do_nivel = list_iterator_next(iterator);
        
        if(pagina_2do_nivel == NULL) {
            list_replace(tabla_paginas_segundo_nivel, i, tabla_segundo_nivel);
            break;
        }

        i++;
    }
    list_iterator_destroy(iterator);

    if(i == list_size(tabla_paginas_segundo_nivel)) {
        list_add(tabla_paginas_segundo_nivel, tabla_segundo_nivel);
        i = list_size(tabla_paginas_segundo_nivel) - 1;
    }*/

    return i;    
}


void liberar_marcos(int id_proceso) {
    int i;

    for (i=0; i<cantidad_de_marcos_totales; i++) {
        if(bitarray_marcos[i] == id_proceso)
            bitarray_marcos[i] = -1;
    }   
}