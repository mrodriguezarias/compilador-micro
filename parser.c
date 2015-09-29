/*
 *  parser.c
 *  Implementación del parser
 */

#include "error.h"
#include "parser.h"

void objetivo(void) {
	//<objetivo> -> <programa> FDT
	programa();
	match(FDT);
	terminar(); //#terminar
}

void programa(void){
	//<programa> -> INICIO <listaSentencias> FIN
	comenzar(); //#comenzar
	match(INICIO);
	lista_sentencias();
	match(FIN);
}

void lista_sentencias(void){
	//<listaSentencias> -> <sentencia> {<sentencia>}
	token tok = next_token();
	sentencia();
	while (1) {
		if (tok == LEER || tok == ESCRIBIR) {
			sentencia();
		}else {
			return;
		}
	}
}

void sentencia(void){
	/*
	*<sentencia> -> ID ASIGNACION <expresión> PUNTOYCOMA |
	*				LEER PARENIZQUIERDO <listaIdentificadores> PARENDERECHO PUNTOYCOMA |
	*				ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA
	*/
	token tok = next_token();
	reg_expr rigth_operand, left_operand;
	switch (tok) {

		case ID:
			identificador(rigth_operand);
			match(ASIGNACION);
			expresion(left_operand);
			asignar(left_operand,rigth_operand); //#asignar
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
			break;
	}
}

void lista_identificadores(void){
	//<listaIdentificadores>-> ID {COMA ID}
	reg_expr id;
	identificador(id);
	leer_id(id); //#leer_id
	while(1){
		if(next_token()==COMA){
			match(COMA);
			identificador(id);
			leer_id(id); //#leer_id
		}else{
			return;
		}
	}
}

void identificador(struct reg_expr *preg){
	match(ID);
	*preg = procesar_id(); //#procesar_id
}

void lista_expresiones(void){
	//<listaExpresiones> -> <expresión> {COMA <expresión>}
	reg_expr exp;
	expresion(exp);
	escribir_exp(exp); //#escribir_exp
	while(1){
		if(next_token()==COMA){
			match(COMA);
			expresion(exp);
			escribir_exp(exp); //#escribir_exp
		}else{
			return;
		}
	}
}

void expresion(struct reg_expr *preg){
	//<expresión>-> <primaria> {<operadorAditivo> <primaria>}
	reg_expr rigth_operand, left_operand;
	reg_op op;
	termino(left_operand);
	while(1){
		token tok = next_token();
		if(tok == SUMA || tok == RESTA){
			operador_aditivo(op);
			termino(rigth_operand);
			left_operand = gen_infijo(left_operand, op, rigth_operand); //#gen_infijo
		}
	}
	*preg = left_operand;
}

void termino(struct reg_expr *preg){
	//<término> -> <expresió> {<operadorMultiplicativo> <término>}
	reg_expr rigth_operand, left_operand;
	reg_op op;
	primaria(left_operand);
	while(1){
		token tok = next_token();
		if(tok == MULTIPLICACION || tok == DIVISION){
			operador_multiplicativo(op);
			primaria(rigth_operand);
			left_operand = gen_infijo(left_operand, op, rigth_operand); //#gen_infijo
		}
	}
	*preg = left_operand;
}

void primaria(struct reg_expr *preg){
	/*<primaria> ->  ID |
	*				CONSTANTE |
	*				PARENIZQUIERDO <expresión> PARENDERECHO
	*/
	token tok = next_token();
	switch (tok) {

		case ID:
			identificador(preg);
			break;

		case CONSTANTE:
			match(CONSTANTE);
			*preg = procesar_cte(); //#procesar_cte
			break;

		case PARENIZQUIERDO:
			match(PARENIZQUIERDO);
			expresion(preg);
			match(PARENDERECHO);
			break;

		default:
			error_sintactico(tok);
			break;
	}
}

void operador_aditivo(struct reg_op *preg){
	//<operadorAditivo> -> uno de SUMA RESTA
	match(tok);
	*preg = procesar_op(); //#procesar_cte
}

void operador_multiplicativo(struct reg_op *preg){
	//<operadorMultiplicativo> -> uno de MULTIPLICACION DIVICION
	match(tok);
	*preg = procesar_op(); //#procesar_cte
}

int main(int argc, const char * argv[]) {

	fout = stdout;
	if (argc < 2) error_arg(); // error faltan argumentos
    if((fin = fopen(argv[1], "r")) == NULL)
		error_apertura_arch(argv[1]); //error al abrir el archivo de entrada

	if (argc > 2) 
		if((fout = fopen(argv[2], "w")) == NULL)
			error_apertura_arch(argv[2]); //error al abrir el archivo de salida

	objetivo();
    
    fclose(fin);
    if (argc > 2) fclose(fout);
    return EXIT_SUCCESS;
}