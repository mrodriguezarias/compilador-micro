/*
 *  error.h
 *  Impresión de mensajes de error en la salida de error estándar
 */

#ifndef error_h
#define error_h

#include "scanner.h"

void error_lexico(token);
void error_sintactico(token, string);
void error_de_archivo(string);

#endif /* error_h */
