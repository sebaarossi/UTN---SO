#include "parser.h"


t_instruccion* parsear_linea(char* linea){
    
    char** array_instruccion;
    linea[strlen(linea)] =  '\0';
    int i = 0;
    t_instruccion* instruccion = instruccion_create(); 
    array_instruccion = string_n_split(linea,2," ");
    

    
   if(strcmp(linea,EXIT) != 0){ // Problemas con el EXIT: Cuando no tengo argumentos, string_split rompe
        char ** parametros_instruccion = string_split(array_instruccion[1]," ");


        while(parametros_instruccion[i] != NULL){
            instruccion->parametros[i] = (uint32_t) atof(parametros_instruccion[i]);
            i++;
        }
        
        string_array_destroy(parametros_instruccion);
    }


    instruccion->ID_instruccion = caracter_segun_instruccion(array_instruccion[0]);
    string_array_destroy(array_instruccion);
    
    return instruccion;
}


void  agregar_instruccion(t_list* lista,t_instruccion* instruccion)
{

    if(instruccion->ID_instruccion == 'N' ){
        list_add(lista,instruccion);
        for(int i = 0; i<instruccion->parametros[0]-1;i++ )
        {
            t_instruccion* noOp = instruccion_create();
            noOp->ID_instruccion = 'N';
            list_add(lista,noOp);
        }
    }
    else {
        list_add(lista,instruccion);
    }
    
}

t_list* cargar_lista(FILE* archivo){
    t_list* lista = list_create();
    char buffer[30] ; // El maximo de caracteres que pueden venir  son 27 (nombre de la instruccion + 2*espacio +
                      // 2 * maximo que se puede representar con 4 bytes ) 

    while(fgets(buffer,sizeof(buffer),archivo) != NULL){
        t_instruccion* instruccion;
        instruccion = parsear_linea(buffer);
        agregar_instruccion(lista,instruccion);
    }

    return lista;
    
}