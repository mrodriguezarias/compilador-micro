/*
 *  error.h
 *  Impresión de mensajes de error en la salida de error estándar
 */

#ifndef error_h
#define error_h

#include "scanner.h"

void error_lexico(token tok);
void error_sintactico(token tok);
void error_de_archivo(string);

#endif /* error_h */
