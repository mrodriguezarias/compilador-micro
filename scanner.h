/*
 *  scanner.h
 *  Implementación del escáner
 */

#ifndef scanner_h
#define scanner_h

#include "common.h"

typedef enum {
    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE,
    PARENIZQUIERDO, PARENDERECHO, PUNTOYCOMA, COMA,
    ASIGNACION, SUMA, RESTA, MULTIPLICACION, DIVISION,
    COMENTARIO, FDT, ERRORLEXICO, ERRORASIG, ERRORCTE, NIL
} token;

extern char * const yytext;
extern unsigned yyline;

token next_token(void);
void match(token);

string get_token_name(token);

#endif /* scanner_h */
