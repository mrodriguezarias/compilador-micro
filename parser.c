/*
 *  parser.c
 *  Implementación del parser
 */
 
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
	sentencia();
	while (1) {
		switch (next_token()) {
			case ID: case LEER: case ESCRIBIR:
				sentencia();
				break;
			default:
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
	switch (tok) {

		case ID:
			identificador();
			match(ASIGNACION);
			termino();
			asignar(); //#asignar
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
	identificador();
	leer_id(); //#leer_id
	while(1){
		if(next_token()==COMA){
			match(COMA);
			identificador();
			leer_id(); //#leer_id
		}else{
			return;
		}
	}
}

void identificador(struct reg_expr *preg){
	match(ID);
	procesar_id(); //#procesar_id
}

void lista_expresiones(void){
	//<listaExpresiones> -> <expresión> {COMA <expresión>}
	expresion();
	escribir_exp(); //#escribir_exp
	while(1){
		if(next_token()==COMA){
			match(COMA);
			expresion();
			escribir_exp(); //#escribir_exp
		}else{
			return;
		}
	}
}

void expresion(struct reg_expr *preg){
	//<expresión>-> <primaria> {<operadorAditivo> <primaria>}
	primaria();
	while(1){
		switch(next_token()){

			case SUMA: case RESTA:
				operador_aditivo();
				primaria();
				gen_infijo(); //#gen_infijo
				break;

			default:
				return;
		}
	}
}

void termino(struct reg_expr *preg){
	//<término> -> <expresió> {<operadorMultiplicativo> <término>}
	expresion();
	while(1){
		token tok = next_token();
		if(tok == MULTIPLICACION || tok == DIVISION){
			operador_multiplicativo();
			termino();
		}
	}
}

void primaria(struct reg_expr *preg){
	/*<primaria> ->  ID |
	*				CONSTANTE |
	*				PARENIZQUIERDO <expresión> PARENDERECHO
	*/
	token tok = next_token();
	switch (tok) {

		case ID:
			identificador();
			break;

		case CONSTANTE:
			match(CONSTANTE);
			procesar_cte(); //#procesar_cte
			break;

		case PARENIZQUIERDO:
			match(PARENIZQUIERDO);
			expresion();
			match(PARENDERECHO);
			break;

		default:
			error_sintactico(tok);
			break;
	}
}

void operador_aditivo(struct reg_op *preg){
	//<operadorAditivo> -> uno de SUMA RESTA
	token tok = next_token();
	if(tok == SUMA || tok == RESTA){
		match(tok);
		procesar_op(); //#procesar_cte
	}else{
		//error
	}
}

void operador_multiplicativo(struct reg_op *preg){
	//<operadorMultiplicativo> -> uno de MULTIPLICACION DIVICION
	token tok = next_token();
	if(tok == MULTIPLICACION || tok == DIVISION){
		match(tok);
		procesar_op(); //#procesar_cte
	}else{
		//error
	}
}