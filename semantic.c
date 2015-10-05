/*
 *  semantic.c
 *  Implementación de las rutinas semánticas
 */

#include "semantic.h"

string opname[] = {"+ADD", "-SUBS", "*MULT", "/DIV"};
unsigned temp_ids = 0;

void generar(string op, string a, string b, string c) {
    if (compiler_mode != ASSEMBLE) return;
    fprintf(fout, "%s", op);
    if (a != NULL) fprintf(fout, " %s", a);
    if (b != NULL) fprintf(fout, ",%s", b);
    if (c != NULL) fprintf(fout, ",%s", c);
    fputs("\n", fout);
}

void comenzar(void) {
    
}

void terminar(void) {
    generar("Stop", NULL, NULL, NULL);
}

void asignar(reg_expr * lv, reg_expr * rv) {
    generar("Store", extraer(rv), extraer(lv), NULL);
}

string extraer(reg_expr * reg) {
    return reg->name;
}

void leer_id(reg_expr * reg) {
    generar("Read", extraer(reg), "Integer", NULL);
}

void escribir_exp(reg_expr * reg) {
    generar("Write", extraer(reg), "Integer", NULL);
}

reg_expr procesar_cte(void) {
    reg_expr reg;
    reg.class = CONSTANTE;
    strcpy(reg.name, yytext);
    reg.value = int_from_string(yytext);
    return reg;
}

reg_expr procesar_id(void) {
    reg_expr reg;
    reg.class = ID;
    strcpy(reg.name, yytext);
    reg.value = 0;
    
    chequear(reg.name);
    return reg;
}

reg_op procesar_op(void) {
    reg_op reg;
    int opcode = -1;
    while (opcode < 4 && *opname[++opcode] != *yytext);
    reg.opcode = opcode;
    return reg;
}

char _temp_id[10];
string temp_id(void) {
    sprintf(_temp_id, "Temp#%d", ++temp_ids);
    return _temp_id;
}

reg_expr gen_infijo(reg_expr * lv, reg_op * op, reg_expr * rv) {
    reg_expr reg;
    reg.class = ID;
    strcpy(reg.name, temp_id());
    reg.value = 0;
    
    generar("Declare", reg.name, "Integer", NULL);
    generar(opname[op->opcode]+1, lv->name, rv->name, reg.name);
    
    return reg;
}
