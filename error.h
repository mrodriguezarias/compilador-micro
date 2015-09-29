#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include "scanner.h"

void error_lexico(token tok);
void error_sintactico(token tok);
void error_arg(void);
void error_apertura_arch(char * path);

#endif // ERROR_H_INCLUDED
