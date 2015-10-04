/*
 *  semantic.h
 *  Implementación de las rutinas semánticas
 */

#ifndef semantic_h
#define semantic_h

#include "symbol.h"

typedef struct {
	token clase;
	char nombre[LEXSIZE];
	int valor;
} reg_expr;

typedef struct {
	int cod_oper;
} reg_op;

void generar(string, string, string, string);
void comenzar(void);
void terminar(void);
void asignar(reg_expr *, reg_expr *);
char * extraer(reg_expr *preg);
void leer_id(reg_expr *pin);
void escribir_exp(reg_expr *pout);
reg_expr procesar_cte(void);
reg_expr procesar_id(void);
reg_op procesar_op(void);
reg_expr gen_infijo(reg_expr *pei, reg_op *op, reg_expr *ped);

#endif /* semantic_h */
