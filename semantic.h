/*
 *  semantic.h
 *  Implementación de las rutinas semánticas
 */

#ifndef semantic_h
#define semantic_h

#include "symbol.h"

typedef struct {
	token class;
	char name[LEXSIZE];
	int value;
} reg_expr;

typedef struct {
	int opcode;
} reg_op;

void generar(string, string, string, string);
void comenzar(void);
void terminar(void);
void asignar(reg_expr *, reg_expr *);
string extraer(reg_expr *);
void leer_id(reg_expr *);
void escribir_exp(reg_expr *);
reg_expr procesar_cte(void);
reg_expr procesar_id(void);
reg_op procesar_op(void);
reg_expr gen_infijo(reg_expr *, reg_op *, reg_expr *);

#endif /* semantic_h */
