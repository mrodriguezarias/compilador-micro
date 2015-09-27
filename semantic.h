#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED

#include "symbol.h"

struct reg_expr {
	token clase;
	char nombre[TAMLEX];
	int valor;
};

struct reg_op {
	int cod_oper;
};

void generar(char *op, char *a, char *b, char *c);
void comenzar(void);
void terminar(void);
void asignar(struct reg_expr *vizq, struct reg_expr *vder);
char *extraer(struct reg_expr *preg);
void leer_id(struct reg_expr *pin);
void escribir_exp(struct reg_expr *pout);
struct reg_expr procesar_cte(void);
struct reg_expr procesar_id(void);
struct reg_op procesar_op(void);
struct reg_expr gen_infijo(struct reg_expr *pei, struct reg_op *op, struct reg_expr *ped);

#endif // SEMANTIC_H_INCLUDED
