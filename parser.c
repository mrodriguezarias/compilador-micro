/*
 *  parser.c
 *  Implementación del parser
 */

#include "parser.h"
#include "error.h"

void objetivo(void) {
	// <objetivo> -> <programa> FDT
	programa();
	match(FDT);
	terminar(); // #terminar
}

void programa(void) {
	// <programa> -> INICIO <listaSentencias> FIN
	comenzar(); // #comenzar
	match(INICIO);
	lista_sentencias();
	match(FIN);
}

void lista_sentencias(void) {
	// <listaSentencias> -> <sentencia> {<sentencia>}
	sentencia();
	while (true) {
        switch (next_token()) {
            case ID:
            case LEER:
            case ESCRIBIR:
                sentencia();
                break;
            default:
                return;
        }
	}
}

void sentencia(void) {
    /*
	 * <sentencia> -> <identificador> ASIGNACION <expresión> PUNTOYCOMA
	 *              | LEER PARENIZQUIERDO <listaIdentificadores> PARENDERECHO PUNTOYCOMA
	 *              | ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA
	 */
	token tok = next_token();
	reg_expr right_operand, left_operand;
	switch (tok) {
            
		case ID:
			identificador(&left_operand);
			match(ASIGNACION);
			expresion(&right_operand);
			asignar(&left_operand, &right_operand); // #asignar
			match(PUNTOYCOMA);
			break;

		case LEER:
			match(LEER);
			match(PARENIZQUIERDO);
			lista_identificadores();
			match(PARENDERECHO);
			match(PUNTOYCOMA);
			break;

		case ESCRIBIR:
			match(ESCRIBIR);
			match(PARENIZQUIERDO);
			lista_expresiones();
			match(PARENDERECHO);
			match(PUNTOYCOMA);
			break;

		default:
			syntax_error(tok, "SENTENCIA");
	}
}

void lista_identificadores(void){
	// <listaIdentificadores> -> <identificador> {COMA <identificador>}
	reg_expr id;
    do {
        identificador(&id);
        leer_id(&id); // #leer_id
    } while(next_token() == COMA && (match(COMA), true));
}

void identificador(reg_expr * reg){
	match(ID);
	*reg = procesar_id(); // #procesar_id
}

void lista_expresiones(void) {
	// <listaExpresiones> -> <expresión> {COMA <expresión>}
	reg_expr exp;
    do {
        expresion(&exp);
        escribir_exp(&exp); // #escribir_exp
    } while(next_token() == COMA && (match(COMA), true));
}

void expresion(reg_expr * reg) {
	// <expresión> -> <termino> {<operadorAditivo> <termino>}
    reg_expr left_operand, right_operand;
    reg_op op;
    token tok;
    
    termino(&left_operand);
    
    while ((tok = next_token()) && (tok == SUMA || tok == RESTA)) {
        operador_aditivo(&op);
        termino(&right_operand);
        left_operand = gen_infijo(&left_operand, &op, &right_operand); // #gen_infijo
    }
    
    *reg = left_operand;
}

void termino(reg_expr * reg) {
	// <término> -> <primaria> {<operadorMultiplicativo> <primaria>}
    reg_expr left_operand, right_operand;
    reg_op op;
    token tok;
    
    primaria(&left_operand);
    
    while ((tok = next_token()) && (tok == MULTIPLICACION || tok == DIVISION)) {
        operador_multiplicativo(&op);
        primaria(&right_operand);
        left_operand = gen_infijo(&left_operand, &op, &right_operand); // #gen_infijo
    }
    
	*reg = left_operand;
}

void primaria(reg_expr * reg) {
    /* <primaria> -> <identificador>
	 *             | CONSTANTE
	 *             | PARENIZQUIERDO <expresión> PARENDERECHO
	 */
	token tok = next_token();
	switch (tok) {
		case ID:
			identificador(reg);
			break;

		case CONSTANTE:
			match(CONSTANTE);
			*reg = procesar_cte(); // #procesar_cte
			break;

		case PARENIZQUIERDO:
			match(PARENIZQUIERDO);
			expresion(reg);
			match(PARENDERECHO);
			break;

		default:
			syntax_error(tok, "PRIMARIA");
	}
}

void operador_aditivo(reg_op * reg) {
	// <operadorAditivo> -> SUMA | RESTA
    token tok = next_token();
    
    if (tok != SUMA && tok != RESTA)
        syntax_error(tok, "OPERADOR_ADITIVO");
    
    match(tok);
	*reg = procesar_op(); // #procesar_op
}

void operador_multiplicativo(reg_op * reg) {
    // <operadorMultiplicativo> -> MULTIPLICACION | DIVISION
    token tok = next_token();
    
    if (tok != MULTIPLICACION && tok != DIVISION)
        syntax_error(tok, "OPERADOR_MULTIPLICATIVO");
    
    match(tok);
    *reg = procesar_op(); // #procesar_op
}
