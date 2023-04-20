
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_utils.h"

t_list *leer_archivo(FILE*, t_log *);
t_list* cargar_lista(FILE* );
void  agregar_instruccion(t_list* ,t_instruccion* );

#endif
