#include "archivos_swap.h"

void crear_archivo_swap(int proceso_id,int tamanio_proceso) {
    char* path_swap= crear_path_swap(proceso_id);    
    FILE* fd_swap= fopen(path_swap, "wb+");
    
    if( !fd_swap )
        perror("No se pudo abrir el archivo");

    ftruncate(fileno(fd_swap),tamanio_proceso);

    free(path_swap);
    fclose(fd_swap);
}


void borrar_archivo_swap(int proceso_id) {
    char* path = crear_path_swap(proceso_id);
    remove(path);
    free(path);
}


char* crear_path_swap(int proceso_id) {
    char* p1= get_path_swap(); //"ruta" "/id_proceso.swap"
	char* p2;
    char* p3= ".swap";

    p2 = string_itoa(proceso_id);
    
	char* path= (char*) calloc((strlen(p1)+strlen(p2)+strlen(p3)),sizeof(char));

	strcat(path, p1);
    strcat(path, p2);
    strcat(path, p3);
    
    return path;
}


void* leer_swap(int proceso_id, uint32_t pagina) {
    void* contenido_pagina = malloc(tam_pagina);

    char* path= crear_path_swap(proceso_id);
    FILE * fd_swap= fopen(path, "rb");
    free(path);

    if(fd_swap == NULL)
        perror("Error al abrir el archivo");

    int posicion_pagina = pagina * tam_pagina;
    fseek(fd_swap, posicion_pagina, SEEK_SET);
    fread(contenido_pagina,tam_pagina,1, fd_swap);
    fclose(fd_swap);

    usleep(retardo_swap*1000);

    return contenido_pagina;
}


void escribir_en_swap( int proceso_id, void* contenido_pagina,int pagina) {
    char* path= crear_path_swap(proceso_id);
	FILE* fd_swap = fopen(path, "r+b");
    
    //Me posiciono en la pagina
    int posicion_pagina= pagina * tam_pagina;
    fseek(fd_swap, posicion_pagina, SEEK_SET);

    fwrite(contenido_pagina, 1, tam_pagina, fd_swap);
    
    fclose(fd_swap);
    free(path);
    
    usleep(retardo_swap*1000);
}
