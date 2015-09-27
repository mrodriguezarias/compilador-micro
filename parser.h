#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "semantic.h"

void objetivo(void);
void programa(void);
void lista_sentencias(void);
void sentencia(void);
void lista_identificadores(void);
void identificador(struct reg_expr *preg);
void lista_expresiones(void);
void expresion(struct reg_expr *preg);
void termino(struct reg_expr *preg);
void primaria(struct reg_expr *preg);
void operador_aditivo(struct reg_op *preg);
void operador_multiplicativo(struct reg_op *preg);

#endif // PARSER_H_INCLUDED
