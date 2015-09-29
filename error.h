#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include "scanner.h"

void error_lexico(token tok);
void error_sintactico(token tok);
void error_de_archivo(const char *);

#endif // ERROR_H_INCLUDED
