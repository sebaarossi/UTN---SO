#include "algoritmos_swap.h" 


int algoritmo_clock(int id_proceso) {
    t_list* lista_marcos_disponibles = obtener_tabla_auxiliar(tabla_primer_nivel_global);
	t_entrada_segundo_nivel* puntero;
	t_entrada_segundo_nivel* proxima_posicion;

	if(list_size(lista_marcos_disponibles) < marcos_por_proceso){
		int marco_libre = buscar_marco_libre(id_proceso);
		return marco_libre;
	}
	bool ordenar_segun_marco(t_entrada_segundo_nivel* primer_entrada,t_entrada_segundo_nivel* segunda_entrada){
		return primer_entrada->marco < segunda_entrada->marco;
	}
	list_sort(lista_marcos_disponibles,(void*) ordenar_segun_marco);

	bool tiene_el_indice(t_entrada_segundo_nivel* entrada){
		return entrada->indice == 1;
	}
	t_entrada_segundo_nivel* entrada_con_indice = list_find(lista_marcos_disponibles,(void*) tiene_el_indice);
	bool menores_que(t_entrada_segundo_nivel* entrada){
		return entrada->marco <  entrada_con_indice->marco;
	}
	t_list* lista_menores = list_filter(lista_marcos_disponibles,(void*)menores_que);

	bool mayores_que(t_entrada_segundo_nivel* entrada){
		return entrada->marco >=  entrada_con_indice->marco;
	}
	t_list* lista_mayores = list_filter(lista_marcos_disponibles,(void*)mayores_que);

	list_clean(lista_marcos_disponibles);

	list_add_all(lista_marcos_disponibles,lista_mayores);
	list_add_all(lista_marcos_disponibles,lista_menores);

	list_destroy(lista_mayores);
	list_destroy(lista_menores);


	//Primera vuelta
	for(int i =0; i < list_size(lista_marcos_disponibles);i++){

		puntero = list_get(lista_marcos_disponibles,i);

		if(puntero->bit_de_uso==0){
			proxima_posicion = list_get(lista_marcos_disponibles,(i+1)%list_size(lista_marcos_disponibles));
			cambiarUltimaPosicion(puntero, proxima_posicion);

			if(puntero->bit_de_modificado == 1) {
					void * contenido_pag = leer_dato_memoria(puntero -> marco);
					int pagina = puntero -> pagina;

					sem_wait(&sem_swap);
					escribir_en_swap(id_proceso, contenido_pag, pagina);
					sem_post(&sem_swap);
				}
			puntero->bit_presencia = 0;
			puntero->bit_de_uso=0;
			puntero->bit_de_modificado =0;
			return puntero->marco;
		}
		else{
			puntero->indice = 0;
			puntero->bit_de_uso = 0;
		}
	}	

	//Segunda vuelta
	for(int i =0; i < list_size(lista_marcos_disponibles);i++){

		puntero = list_get(lista_marcos_disponibles,i);

		if(puntero->bit_de_uso==0){
			proxima_posicion = list_get(lista_marcos_disponibles,(i+1)%list_size(lista_marcos_disponibles));
			cambiarUltimaPosicion(puntero, proxima_posicion);

			if(puntero->bit_de_modificado == 1) {
					void * contenido_pag = leer_dato_memoria(puntero -> marco);
					int pagina = puntero -> pagina;

					sem_wait(&sem_swap);
					escribir_en_swap(id_proceso, contenido_pag, pagina);
					sem_post(&sem_swap);
				}
			puntero->bit_presencia = 0;
			puntero->bit_de_uso=0;
			puntero->bit_de_modificado =0;
			return puntero->marco;
		}
		else
			puntero->indice = 0;
	}	

	log_error(logger,"Error en algoritmo clock");
	return -1;
}


t_list* obtener_tabla_auxiliar(int tabla_primer_nivel_global) {
    t_pagina_primer_nivel* tabla_primer_nivel = list_get(tabla_paginas_primer_nivel,tabla_primer_nivel_global);
	t_list* lista_auxiliar = list_create();
    // Armo tabla aux de entradas de segundo nivel
    for(int i=0; i < list_size(tabla_primer_nivel->entradas_primer_nivel);i++){
        int* entrada = list_get(tabla_primer_nivel->entradas_primer_nivel,i);
        t_pagina_segundo_nivel* tabla_segundo_nivel = list_get(tabla_paginas_segundo_nivel, *entrada);

        t_list_iterator* iterator = list_iterator_create(tabla_segundo_nivel->entradas_segundo_nivel);

		while(list_iterator_has_next(iterator)){
			t_entrada_segundo_nivel* ent_2do_nivel = list_iterator_next(iterator);
		
			if(ent_2do_nivel->bit_presencia == 1){
				list_add(lista_auxiliar,ent_2do_nivel);
			}
		}
    }
	return lista_auxiliar;
}


int algoritmo_clock_modificado(int id_proceso) {
	t_list* lista_marcos_disponibles = obtener_tabla_auxiliar(tabla_primer_nivel_global);
	t_entrada_segundo_nivel* puntero;
	t_entrada_segundo_nivel* proxima_posicion;

	if(list_size(lista_marcos_disponibles) < marcos_por_proceso){
		int marco_libre = buscar_marco_libre(id_proceso);
		return marco_libre;
	}
	bool ordenar_segun_marco(t_entrada_segundo_nivel* primer_entrada,t_entrada_segundo_nivel* segunda_entrada){
		return primer_entrada->marco < segunda_entrada->marco;
	}
	list_sort(lista_marcos_disponibles,(void*) ordenar_segun_marco);

	bool tiene_el_indice(t_entrada_segundo_nivel* entrada){
		return entrada->indice == 1;
	}
	t_entrada_segundo_nivel* entrada_con_indice = list_find(lista_marcos_disponibles,(void*) tiene_el_indice);
	bool menores_que(t_entrada_segundo_nivel* entrada){
		return entrada->marco <  entrada_con_indice->marco;
	}
	t_list* lista_menores = list_filter(lista_marcos_disponibles,(void*)menores_que);

	bool mayores_que(t_entrada_segundo_nivel* entrada){
		return entrada->marco >=  entrada_con_indice->marco;
	}
	t_list* lista_mayores = list_filter(lista_marcos_disponibles,(void*)mayores_que);

	list_clean(lista_marcos_disponibles);

	list_add_all(lista_marcos_disponibles,lista_mayores);
	list_add_all(lista_marcos_disponibles,lista_menores);

	list_destroy(lista_mayores);
	list_destroy(lista_menores);
	//Primera vuelta, me fijo si hay alguno (0,0) sin modificar nada
	for(int i =0; i < list_size(lista_marcos_disponibles);i++){

		puntero = list_get(lista_marcos_disponibles,i);

		if(puntero->bit_de_uso==0 && puntero->bit_de_modificado==0){
			proxima_posicion = list_get(lista_marcos_disponibles,(i+1)%list_size(lista_marcos_disponibles));
			cambiarUltimaPosicion(puntero, proxima_posicion);

			puntero->bit_presencia = 0;
			return puntero->marco;
		}
		puntero->indice = 0;

	}	

	
	//Segunda vuelta
	for(int i =0; i < list_size(lista_marcos_disponibles);i++){

		puntero = list_get(lista_marcos_disponibles,i);

		if(puntero->bit_de_uso==0 && puntero->bit_de_modificado==1) {
			proxima_posicion = list_get(lista_marcos_disponibles,(i+1)%list_size(lista_marcos_disponibles));
			cambiarUltimaPosicion(puntero, proxima_posicion);
			// Mutex
			if(puntero->bit_de_modificado == 1) {
				void * contenido_pag = leer_dato_memoria(puntero -> marco);
				int pagina = puntero -> pagina;

				sem_wait(&sem_swap);
				escribir_en_swap(id_proceso, contenido_pag, pagina);
				sem_post(&sem_swap);
			}
			puntero->bit_presencia = 0;
			puntero->bit_de_uso=0;
			puntero->bit_de_modificado =0;
			return puntero->marco;
		}else{
			puntero->bit_de_uso=0;
			puntero->indice = 0;
		}
	}
	// Tercer vuelta, chequeo 0 y 0
    for(int i =0; i < list_size(lista_marcos_disponibles);i++){

		puntero = list_get(lista_marcos_disponibles,i);

		if(puntero->bit_de_uso==0 && puntero->bit_de_modificado == 0) {
			proxima_posicion = list_get(lista_marcos_disponibles,(i+1)%list_size(lista_marcos_disponibles));
			cambiarUltimaPosicion(puntero, proxima_posicion);
			
			puntero->bit_presencia = 0;

			return puntero->marco;
		}else{
			puntero->bit_de_uso=0;
			puntero->indice =0;		
		}
	}
	// Cuarta vuelta, chequeo 0 y 1
	for(int i =0; i < list_size(lista_marcos_disponibles);i++){

		puntero = list_get(lista_marcos_disponibles,i);

		if(puntero->bit_de_uso==0 && puntero->bit_de_modificado == 1) {
			proxima_posicion = list_get(lista_marcos_disponibles,(i+1)%list_size(lista_marcos_disponibles));
			cambiarUltimaPosicion(puntero, proxima_posicion);
			// Mutex
			if(puntero->bit_de_modificado == 1) {
				void * contenido_pag = leer_dato_memoria(puntero -> marco);
				int pagina = puntero -> pagina;
				sem_wait(&sem_swap);
				escribir_en_swap(id_proceso, contenido_pag, pagina);
				sem_post(&sem_swap);
				free(contenido_pag);
			}
			puntero->bit_presencia = 0;
			puntero->bit_de_modificado =0;
			return puntero->marco;
		}else{
			puntero->bit_de_uso=0;
			puntero->indice =0;		
		}
	}

	log_error(logger,"Error en algoritmo clock modificado");
	return -1;
}


int buscar_marco_libre(int id_proceso) {
	int i;

	for(i=0 ; i<cantidad_de_marcos_totales ; i++) {
		if(bitarray_marcos[i] == -1) {
			bitarray_marcos[i] = id_proceso;
			break;
		}
	}

	return i;
}


int buscarUltimaPosSacada(t_list* lista_marcos_disponibles) {
	int pos=0;
	int size = list_size(lista_marcos_disponibles);

	for(pos=0; pos<size ; pos++) {
		t_entrada_segundo_nivel* entrada = list_get(lista_marcos_disponibles, pos);
		if(entrada->indice == 1)
			break;
	}

	return pos;
}

void* leer_dato_memoria(uint32_t marco){
    void * valor_memoria = malloc(tam_pagina);
    memcpy(valor_memoria,memoria_ram + marco * tam_pagina,tam_pagina);
    return valor_memoria;
}

void cambiarUltimaPosicion(t_entrada_segundo_nivel* puntero, t_entrada_segundo_nivel* proxima_posicion){
	puntero->indice=0; //la posicion que te paso
	proxima_posicion->indice=1; //una posicion mas
}