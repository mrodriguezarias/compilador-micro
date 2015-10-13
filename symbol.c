/*
 *  symbol.c
 *  Tabla de símbolos y funciones relacionadas
 */

#include "symbol.h"
#include "semantic.h"

#define symbol_size 1024
unsigned symbol_length = 0;

symbol symbols[symbol_size];

/* Inicializa la tabla de símbolos con las palabras reservadas.
 */
void build_symbol_table(void) {
    if (symbol_length > 0) return;
    symbol * sym;
    do {
        sym = symbols + symbol_length;
        sym->tok = (token) symbol_length;
        strcpy(sym->lex, get_token_name(sym->tok));
        change_string_case(sym->lex, lowercase);
    } while (++symbol_length < 4);
}

/* Busca en la tabla de símbolos uno que tenga lexema igual a lex.
 * Si lo encuentra devuelve true y guarda en tok el token del
 * símbolo hallado.
 */
bool buscar(string lex, symbol * symbols, token * tok) {
    symbol * sym;
    for (unsigned i = 0; i < symbol_length && (sym = symbols + i); i++) {
        if (!strings_are_equal(lex, sym->lex)) continue;
        if (tok != NULL) *tok = sym->tok;
        return true;
    }
    return false;
}

/* Agrega a la tabla de símbolos un identificador de lexema lex.
 */
void colocar(string lex, symbol * symbols) {
    generar("Declare", lex, "Integer", NULL);
    symbol * sym = symbols + symbol_length++;
    strcpy(sym->lex, lex);
    sym->tok = ID;
}

/* Comprueba si en la tabla de símbolos existe un identificador cuyo
 * lexema es lex. Si no lo encuentra, lo agrega a la tabla.
 */
void chequear(string lex) {
    if (!buscar(lex, symbols, NULL))
        colocar(lex, symbols);
}
