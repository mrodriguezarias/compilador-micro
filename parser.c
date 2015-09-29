/*
 *  parser.c
 *  Implementación del parser
 */

#include "error.h"
#include "parser.h"

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
	while (1) {
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
	struct reg_expr right_operand, left_operand;
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
			error_sintactico(tok);
	}
}

void lista_identificadores(void){
	// <listaIdentificadores> -> <identificador> {COMA <identificador>}
	struct reg_expr id;
    do {
        identificador(&id);
        leer_id(&id); // #leer_id
    } while(next_token() == COMA && (match(COMA), true));
}

void identificador(struct reg_expr * preg){
	match(ID);
	*preg = procesar_id(); // #procesar_id
}

void lista_expresiones(void) {
	// <listaExpresiones> -> <expresión> {COMA <expresión>}
	struct reg_expr exp;
    do {
        expresion(&exp);
        escribir_exp(&exp); // #escribir_exp
    } while(next_token() == COMA && (match(COMA), true));
}

void expresion(struct reg_expr * preg) {
	// <expresión> -> <termino> {<operadorAditivo> <termino>}
    struct reg_expr left_operand, right_operand;
    struct reg_op op;
    token tok;
    
    termino(&left_operand);
    
    while ((tok = next_token()) && (tok == SUMA || tok == RESTA)) {
        operador_aditivo(&op);
        termino(&right_operand);
        left_operand = gen_infijo(&left_operand, &op, &right_operand); // #gen_infijo
    }
    
    *preg = left_operand;
}

void termino(struct reg_expr * preg) {
	// <término> -> <primaria> {<operadorMultiplicativo> <primaria>}
    struct reg_expr left_operand, right_operand;
    struct reg_op op;
    token tok;
    
    primaria(&left_operand);
    
    while ((tok = next_token()) && (tok == MULTIPLICACION || tok == DIVISION)) {
        operador_multiplicativo(&op);
        primaria(&right_operand);
        left_operand = gen_infijo(&left_operand, &op, &right_operand); // #gen_infijo
    }
    
	*preg = left_operand;
}

void primaria(struct reg_expr * preg) {
    /* <primaria> -> <identificador>
	 *             | CONSTANTE
	 *             | PARENIZQUIERDO <expresión> PARENDERECHO
	 */
	token tok = next_token();
	switch (tok) {
		case ID:
			identificador(preg);
			break;

		case CONSTANTE:
			match(CONSTANTE);
			*preg = procesar_cte(); // #procesar_cte
			break;

		case PARENIZQUIERDO:
			match(PARENIZQUIERDO);
			expresion(preg);
			match(PARENDERECHO);
			break;

		default:
			error_sintactico(tok);
	}
}

void operador_aditivo(struct reg_op * preg) {
	// <operadorAditivo> -> SUMA | RESTA
    token tok = next_token();
    
    if (tok != SUMA && tok != RESTA)
        error_sintactico(tok);
    
    match(tok);
	*preg = procesar_op(); // #procesar_op
}

void operador_multiplicativo(struct reg_op * preg) {
    // <operadorMultiplicativo> -> MULTIPLICACION | DIVISION
    token tok = next_token();
    
    if (tok != MULTIPLICACION && tok != DIVISION)
        error_sintactico(tok);
    
    match(tok);
    *preg = procesar_op(); // #procesar_op
}

int main(int argc, const char * argv[]) {
    fin = stdin;
    fout = stdout;
    if (argc > 1) fin = fopen(argv[1], "r");
    if (argc > 2) fout = fopen(argv[2], "w");
    if(fin == NULL || ferror(fin)) error_de_archivo(argv[1]);
    if(fout == NULL || ferror(fout)) error_de_archivo(argv[2]);
    
	objetivo();
    
    fclose(fin);
    if (argc > 2) fclose(fout);
    return EXIT_SUCCESS;
}