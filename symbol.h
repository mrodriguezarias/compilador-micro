/*
 *  symbol.h
 *  Tabla de símbolos y funciones relacionadas
 */

#ifndef symbol_h
#define symbol_h

#include "scanner.h"

#define LEXSIZE 32+1

typedef struct {
	char lex[LEXSIZE];
	token tok;
} symbol;

extern symbol symbols[];

void build_symbol_table(void);
bool buscar(string, symbol *, token *);
void colocar(string, symbol *);
void chequear(string);

#endif /* symbol_h */
