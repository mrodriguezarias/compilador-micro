/*
 *  error.c
 *  Impresión de mensajes de error en la salida de error estándar
 */

#include "error.h"
#include <errno.h>
 
void lexical_error(token tok) {
    fprintf(ferr, "Error léxico en la línea %u: ", yyline);
    switch (tok) {
        case ERRORLEXICO:
            fprintf(ferr, "el lexema '%s' no pertenece a ninguna categoría léxica.\n", yytext);
            break;
        case ERRORASIG:
            fprintf(ferr, "'%s' no es un operador de asignación bien formado.\n", yytext);
            break;
        case ERRORCTE:
            fprintf(ferr, "'%s' no es una constante numérica bien formada.\n", yytext);
            break;
        default:
            break;
    }
    exit(EXIT_FAILURE);
}

void syntax_error(token tok, string cat) {
    fprintf(ferr, "Error sintáctico en la línea %u: ", yyline);
    if (cat == NULL)
        fprintf(ferr, "el lexema '%s' no pertenece a la categoría léxica %s.\n", yytext, get_token_name(tok));
    else
        fprintf(ferr, "la categoría sintáctica %s no puede comenzar con '%s'.\n", cat, yytext);
    exit(EXIT_FAILURE);
}

void file_error(string nombre) {
    fprintf(ferr, "Error al procesar el archivo '%s': %s.\n", nombre, strerror(errno));
	exit(EXIT_FAILURE);
}

void invalid_argument(string arg) {
    if (arg != NULL)
        fprintf(ferr, "Error: '%s' no es un argumento válido.\n", arg);
    else
        fputs("Error: faltan argumentos.\n", ferr);
    exit(EXIT_FAILURE);
}
