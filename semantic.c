/*
 *  semantic.c
 *  Implementación de las rutinas semánticas
 */

#include "semantic.h"

void generar(char *op, char *a, char *b, char *c) {
    // Esta es la que hay que hacer para la entrega de mañana (2015-09-30)
}

void comenzar(void) {
    
}

void terminar(void) {
    
}

void asignar(struct reg_expr *lval, struct reg_expr *rval) {
    
}

char * extraer(struct reg_expr *preg) {
    return NULL;
}

void leer_id(struct reg_expr *pin) {
    
}

void escribir_exp(struct reg_expr *pout) {
    
}

struct reg_expr procesar_cte(void) {
    return (struct reg_expr){};
}

struct reg_expr procesar_id(void) {
    return (struct reg_expr){};
}

struct reg_op procesar_op(void) {
    return (struct reg_op){};
}

struct reg_expr gen_infijo(struct reg_expr *pei, struct reg_op *op, struct reg_expr *ped) {
    return (struct reg_expr){};
}
