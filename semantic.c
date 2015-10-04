/*
 *  semantic.c
 *  Implementación de las rutinas semánticas
 */

#include "semantic.h"

void generar(string op, string a, string b, string c) {
    fprintf(fout, "%s", op);
    if (a != NULL) fprintf(fout, " %s", a);
    if (b != NULL) fprintf(fout, ",%s", b);
    if (c != NULL) fprintf(fout, ",%s", c);
    fputs("\n", fout);
}

void comenzar(void) {
    
}

void terminar(void) {
    
}

void asignar(reg_expr *lval, reg_expr *rval) {
    
}

char * extraer(reg_expr *preg) {
    return NULL;
}

void leer_id(reg_expr *pin) {
    
}

void escribir_exp(reg_expr *pout) {
    
}

reg_expr procesar_cte(void) {
    return (reg_expr){};
}

reg_expr procesar_id(void) {
    return (reg_expr){};
}

reg_op procesar_op(void) {
    return (reg_op){};
}

reg_expr gen_infijo(reg_expr *pei, reg_op *op, reg_expr *ped) {
    return (reg_expr){};
}
