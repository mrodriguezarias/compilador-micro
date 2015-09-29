/*
 *  error.c
 *  Mostrar errores
 */

#include "error.h"
 
void error_lexico(token tok) {
    fprintf(fout, "Error léxico en la línea %u: ", yyline);
    switch (tok) {
        case ERRORLEXICO:
            fprintf(fout, "el lexema '%s' no pertenece a ninguna categoría léxica", yytext);
            break;
        case ERRORASIG:
            fprintf(fout, "'%s' no es un operador de asignación bien formado", yytext);
            break;
        case ERRORCTE:
            fprintf(fout, "'%s' no es una constante numérica bien formada", yytext);
            break;
        default:
            break;
    }
    fputs(".\n", fout);
    exit(EXIT_FAILURE);
}

void error_sintactico(token tok) {
    fprintf(fout, "Error sintáctico en la línea %u: el lexema '%s' no pertenece a la categoría léxica %s.\n", yyline, yytext, get_token_name(tok));
    exit(EXIT_FAILURE);
}

void error_arg(void) {
	fprintf(fout, "parser debe recibir por lo menos 1 argumento.\n\tparcer <archivo entrada> [<archivo salida>]\n");
	exit(EXIT_FAILURE);
}
void error_apertura_arch(char * path) {
	fprintf(fout, "Error en la apertura del archivo %s\n", *path);
	exit(EXIT_FAILURE);
}