/*
 *  error.c
 *  Mostrar errores
 */

#include "error.h"
#include <errno.h>
 
void error_lexico(token tok) {
    fprintf(stderr, "Error léxico en la línea %u: ", yyline);
    switch (tok) {
        case ERRORLEXICO:
            fprintf(stderr, "el lexema '%s' no pertenece a ninguna categoría léxica.\n", yytext);
            break;
        case ERRORASIG:
            fprintf(stderr, "'%s' no es un operador de asignación bien formado.\n", yytext);
            break;
        case ERRORCTE:
            fprintf(stderr, "'%s' no es una constante numérica bien formada.\n", yytext);
            break;
        default:
            break;
    }
    exit(EXIT_FAILURE);
}

void error_sintactico(token tok) {
    fprintf(stderr, "Error sintáctico en la línea %u: el lexema '%s' no pertenece a la categoría léxica %s.\n", yyline, yytext, get_token_name(tok));
    exit(EXIT_FAILURE);
}

void error_de_archivo(const char * nombre) {
    fprintf(stderr, "Error al procesar archivo '%s': %s.\n", nombre, strerror(errno));
	exit(EXIT_FAILURE);
}