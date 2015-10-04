/*
 *  parser.h
 *  Implementación del parser
 */

#ifndef parser_h
#define parser_h

#include "semantic.h"

void objetivo(void);
void programa(void);
void lista_sentencias(void);
void sentencia(void);
void lista_identificadores(void);
void identificador(reg_expr *);
void lista_expresiones(void);
void expresion(reg_expr *);
void termino(reg_expr *);
void primaria(reg_expr *);
void operador_aditivo(reg_op *);
void operador_multiplicativo(reg_op *);

#endif /* parser_h */
