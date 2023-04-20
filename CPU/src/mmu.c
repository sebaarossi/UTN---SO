#include "mmu.h"

uint32_t traducir_direccion(uint32_t direccion_logica, t_pcb* pcb) {
    uint32_t numero_pagina;
    uint32_t entrada_tabla_1er_nivel;
    uint32_t entrada_tabla_2do_nivel;
    uint32_t desplazamiento;
    uint32_t direccion_fisica;

    numero_pagina = floorl(direccion_logica / tam_pagina);
   
    int32_t marco = obtener_marco(pcb->id_proceso,numero_pagina); // TLB

    if(marco == -1) {
        entrada_tabla_1er_nivel = floorl(numero_pagina / entradas_por_tabla);
        int tabla_segundo_nivel = obtener_tabla_segundo_nivel(pcb->tabla_paginas, entrada_tabla_1er_nivel);
        
        entrada_tabla_2do_nivel = numero_pagina % entradas_por_tabla;
        int marco = obtener_marco_memoria(tabla_segundo_nivel, entrada_tabla_2do_nivel,numero_pagina);

        desplazamiento = direccion_logica - numero_pagina * tam_pagina;
        direccion_fisica = marco*tam_pagina + desplazamiento;

        agregar_entrada_tlb(pcb->id_proceso, numero_pagina, marco);
        log_info(logger,"NEW ENTRY TLB: Pagina %d, Marco: %d ",numero_pagina, marco);
    }
    else {
        desplazamiento = direccion_logica - numero_pagina * tam_pagina;
        direccion_fisica = marco*tam_pagina + desplazamiento;
    }

    return direccion_fisica;
}


int obtener_tabla_segundo_nivel(int tabla_paginas, int entrada_tabla_1er_nivel) {
    t_paquete* paquete;

    paquete = crear_paquete_con_codigo_especial(ACC_TAB_1);

    agregar_a_paquete(paquete, &tabla_paginas, sizeof(int));
    agregar_a_paquete(paquete, &entrada_tabla_1er_nivel, sizeof(int));

    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);

    recibir_operacion(socket_memoria);
    t_list* lista = recibir_paquete(socket_memoria);

    int* aux_tabla_segundo_nivel = list_remove(lista, 0);
    int tabla_segundo_nivel = *aux_tabla_segundo_nivel;

    free(aux_tabla_segundo_nivel);
    list_destroy(lista);

    return tabla_segundo_nivel;
}


int obtener_marco_memoria(int tabla_segundo_nivel, int entrada_tabla_2do_nivel,uint32_t numero_pagina) {
    t_paquete* paquete;

    paquete = crear_paquete_con_codigo_especial(ACC_TAB_2);

    agregar_a_paquete(paquete, &tabla_segundo_nivel, sizeof(int));
    agregar_a_paquete(paquete, &entrada_tabla_2do_nivel, sizeof(int));
    agregar_a_paquete(paquete, &numero_pagina, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);

    recibir_operacion(socket_memoria);
    t_list* lista = recibir_paquete(socket_memoria);

    int* aux_marco = list_remove(lista, 0);
    int marco = *aux_marco;

    free(aux_marco);
    list_destroy(lista);

    return marco;
}