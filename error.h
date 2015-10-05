/*
 *  error.h
 *  Impresión de mensajes de error en la salida de error estándar
 */

#ifndef error_h
#define error_h

#include "scanner.h"

void lexical_error(token);
void syntax_error(token, string);

void file_error(string);
void invalid_argument(string);

#endif /* error_h */
