/*
 *  common.c
 *  Funciones auxiliares compartidas
 */

#include "common.h"

FILE * fin = NULL, * fout = NULL;

/* Convierte las letras de una cadena a mayúsculas o a minúsculas,
 * según lo especificado por el segundo parámetro.
 */
void change_string_case(char * const str, case_type ct) {
    for (int i = 0, c; (c = str[i]); i++)
        str[i] = ct ? toupper(c) : tolower(c);
}

/* Determina si dos cadenas de texto son iguales.
 */
bool strings_are_equal(string str1, string str2) {
    return strcmp(str1, str2) == 0;
}

/* Busca una cadena dentro de un array de cadenas null-terminated.
 * Si la encuentra devuelve su índice, de lo contrario retorna -1.
 */
int find_string_in_array(string needle, string * haystack) {
    string str;
    for (int i = 0; (str = haystack[i]); i++)
        if (strings_are_equal(str, needle))
            return i;
    return -1;
}

/* Convierte un número entero a una cadena de texto.
 */
char _string_from_int_buffer[11];
const char * string_from_int(int integer) {
    sprintf(_string_from_int_buffer, "%d", integer);
    return _string_from_int_buffer;
}

/* Devuelve un carácter ancho (de cuatro bytes) en la posición indicada
 * de la cadena pasada por parámetro.
 */
char _wide_char_at_buffer[4];
const char * wide_char_at(int pos, string str) {
    int i;
    for (i = 0; i < 3; i++)
        _wide_char_at_buffer[i] = str[3 * pos + i];
    _wide_char_at_buffer[i] = '\0';
    return _wide_char_at_buffer;
}
