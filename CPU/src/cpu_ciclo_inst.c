#include "cpu_ciclo_inst.h"


struct timespec start,stop;


//////////////////////////////////////////////////////////
/***************** Ciclo de Instruccion *****************/
//////////////////////////////////////////////////////////

void correr_ciclo_instruccion(t_pcb * pcb) {
    clock_gettime(CLOCK_REALTIME, &start);
    
    // Caso base
    log_info(logger,"Ejecuto proceso: %d",pcb->id_proceso);
    t_instruccion* instruccion = determinar_siguiente_instruccion(pcb);

    if(instruccion->ID_instruccion == 'C')
        fetch_operands(instruccion, pcb);
   
    ejecutar_instruccion(instruccion,pcb);
    
    if(instruccion->ID_instruccion == 'E' || instruccion ->ID_instruccion == 'I')
        return; 
    
    while(get_checkeo_interrupt() == 0 ) {

        instruccion = determinar_siguiente_instruccion(pcb);
        
        if(instruccion->ID_instruccion == 'C')
            fetch_operands(instruccion, pcb);

        ejecutar_instruccion(instruccion, pcb);

        if(instruccion->ID_instruccion == 'E' || instruccion->ID_instruccion == 'I')
            break;
    }
    
    if(get_checkeo_interrupt() == 1) {
        paqueteDispatch = crear_paquete_con_codigo_especial(INTERRUPT);

        clock_gettime(CLOCK_REALTIME, &stop);
        pcb->ultima_rafaga = (double)(stop.tv_sec - start.tv_sec) * 1000 
                            + (double)(stop.tv_nsec - start.tv_nsec) / 1000000;
        pcb->ultima_rafaga_real = pcb->ultima_rafaga_real + pcb->ultima_rafaga; 

        agregar_a_paquete_pcb(paqueteDispatch,pcb);
    }
}

t_instruccion* determinar_siguiente_instruccion (t_pcb * pcb) {
    t_instruccion* proxima_instruccion =  list_get(pcb->instrucciones, pcb->pc);
    pcb-> pc = pcb->pc + 1;
    return proxima_instruccion;
}


void ejecutar_instruccion(t_instruccion* instruccion, t_pcb * pcb) { 
    switch(instruccion->ID_instruccion){
        case 'N':
            ejecutar_instruccion_NOOP();
            break;
        case 'I':
            ejecutar_instruccion_IO(instruccion,pcb);
            break;
        case 'R':
            ejecutar_instruccion_READ(instruccion, pcb);
            break;
        case 'W':
            ejecutar_instruccion_WRITE(instruccion, pcb);
            break;
        case 'C':
            ejecutar_instruccion_COPY(instruccion, pcb, *valor_copy);
            break;
        case 'E':
            ejecutar_instruccion_EXIT(pcb);
            break;
    }
}


void fetch_operands(t_instruccion* instruccion, t_pcb* pcb) {
    uint32_t direccion_logica_origen = instruccion -> parametros[1];
    uint32_t direc_fisica = traducir_direccion(direccion_logica_origen, pcb);
    
    //Enviar por paquete los datos
    t_paquete* paquete_FETOP_direc = crear_paquete_con_codigo_especial(FET_OP);
    agregar_a_paquete(paquete_FETOP_direc, &direc_fisica, sizeof(uint32_t));
    enviar_paquete(paquete_FETOP_direc,socket_memoria);
    eliminar_paquete(paquete_FETOP_direc);
    
    recibir_operacion(socket_memoria);
    t_list* lista = recibir_paquete(socket_memoria);
    valor_copy  = list_remove(lista,0);
}


void ejecutar_instruccion_NOOP() { 
    int tiempo_espera = config_get_int_value(config,"RETARDO_NOOP");
    usleep(tiempo_espera* 1000);
}

void ejecutar_instruccion_IO(t_instruccion* instruccion,t_pcb* pcb) {
    int tiempoDeBloqueo = instruccion->parametros[0];
    paqueteDispatch = crear_paquete_con_codigo_especial(IO_CPU);

    clock_gettime(CLOCK_REALTIME, &stop);
    pcb->ultima_rafaga = (double)(stop.tv_sec - start.tv_sec) * 1000 
                        + (double)(stop.tv_nsec - start.tv_nsec) / 1000000;
    pcb->ultima_rafaga_real = pcb->ultima_rafaga_real + pcb->ultima_rafaga;

    id_proceso_actual = -1;
    agregar_a_paquete(paqueteDispatch,&tiempoDeBloqueo,sizeof(int));
    agregar_a_paquete_pcb(paqueteDispatch,pcb); 
}

void ejecutar_instruccion_EXIT(t_pcb * pcb) {
     id_proceso_actual = -1;
    paqueteDispatch = crear_paquete_con_codigo_especial(EXIT_CPU);
    agregar_a_paquete_pcb(paqueteDispatch,pcb);
}

void ejecutar_instruccion_READ(t_instruccion* instruccion, t_pcb * pcb) {
    uint32_t direccion_logica_read = instruccion -> parametros[0];
	uint32_t direc_fisica_read = traducir_direccion(direccion_logica_read, pcb);

    log_info(logger, "PED_LEC: operacion enviada a memoria");
    t_paquete* paquete_READ = crear_paquete_con_codigo_especial(PED_LEC);
    agregar_a_paquete(paquete_READ, &direc_fisica_read, sizeof(uint32_t));
    enviar_paquete(paquete_READ, socket_memoria);
    eliminar_paquete(paquete_READ);

    recibir_operacion(socket_memoria);
    t_list* lista = recibir_paquete(socket_memoria);
    uint32_t* valor_a_escribir = list_remove(lista,0);
    log_info(logger,"Valor leido: %d ",*valor_a_escribir);
    list_destroy(lista);
    free(valor_a_escribir);
}


void ejecutar_instruccion_WRITE(t_instruccion* instruccion, t_pcb * pcb) {
    uint32_t direccion_logica_write = instruccion -> parametros[0];
    uint32_t valor = instruccion -> parametros[1];
    uint32_t direc_fisica_write = traducir_direccion(direccion_logica_write, pcb);

    log_info(logger, "PED_ESC: operacion enviada");
    t_paquete* paquete_WRITE = crear_paquete_con_codigo_especial(PED_ESC);
    agregar_a_paquete(paquete_WRITE, &direc_fisica_write, sizeof(uint32_t));
    agregar_a_paquete(paquete_WRITE, &valor, sizeof(uint32_t));
    enviar_paquete(paquete_WRITE,socket_memoria);
    eliminar_paquete(paquete_WRITE);

    recibir_operacion(socket_memoria);
    recibir_mensaje(socket_memoria,logger);
}


void ejecutar_instruccion_COPY(t_instruccion* instruccion, t_pcb * pcb, uint32_t valor_copy) {
    uint32_t direccion_logica_destino = instruccion -> parametros[0];
	uint32_t direc_fisica_dest = traducir_direccion(direccion_logica_destino, pcb);

    log_info(logger, "PED_COPY: operacion enviada");
    t_paquete* paquete_COPY = crear_paquete_con_codigo_especial(PED_COPY);
    agregar_a_paquete(paquete_COPY, &direc_fisica_dest, sizeof(uint32_t));
    agregar_a_paquete(paquete_COPY, &valor_copy, sizeof(uint32_t));
    enviar_paquete(paquete_COPY, socket_memoria);

    eliminar_paquete(paquete_COPY);
}

int get_checkeo_interrupt() {
    pthread_mutex_lock(&mutex_interrupt);
    int aux = chequeo_interrupt;
    pthread_mutex_unlock(&mutex_interrupt);
    
    return aux;
}