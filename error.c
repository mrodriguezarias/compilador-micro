/*
 *  error.c
 *  Impresión de mensajes de error en la salida de error estándar
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

void error_sintactico(token tok, string cat) {
    fprintf(stderr, "Error sintáctico en la línea %u: ", yyline);
    string lex = *yytext == FDT ? yytext : "(fin de archivo)";
    if (cat == NULL)
        fprintf(stderr, "el lexema '%s' no pertenece a la categoría léxica %s.\n", lex, get_token_name(tok));
    else
        fprintf(stderr, "la categoría sintáctica %s no puede comenzar con '%s'.\n", cat, lex);
    exit(EXIT_FAILURE);
}

void error_de_archivo(string nombre) {
    fprintf(stderr, "Error al procesar el archivo '%s': %s.\n", nombre, strerror(errno));
	exit(EXIT_FAILURE);
}