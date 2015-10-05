/*
 *  scanner.c
 *  Implementación del escáner
 */

#include "scanner.h"
#include "error.h"
#include "symbol.h"

string tok_names[] = {
    "INICIO", "FIN", "LEER", "ESCRIBIR", "ID", "CONSTANTE",
    "PARENIZQUIERDO", "PARENDERECHO", "PUNTOYCOMA", "COMA",
    "ASIGNACION", "SUMA", "RESTA", "MULTIPLICACION", "DIVISION",
    "COMENTARIO", "FDT", "ERRORLEXICO", "ERRORASIG", "ERRORCTE", NULL
};

static char lexeme[128];
char * const yytext = lexeme;
unsigned yyline = 1, lexindex = 0;

token current_token = NIL;
bool read_next_token = true;

/* Tabla de transiciones
 */
unsigned transition_table[][16] = {
    { 9, 10, 11, 12,  4, 20, 14, 15, 16,  5,  0, 19,  1,  2,  0, 20},
    { 7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  1,  1,  7,  7},
    { 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  3,  2,  8,  8},
    {22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,  3,  3, 22, 22},
    {21, 21, 21, 21, 21, 13, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},
    {17, 17, 17, 17, 17, 17, 17, 17, 17,  6, 17, 17, 17, 17, 17, 17},
    { 6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 18, 18,  6,  6,  6,  6}
};

/* Devuelve el nombre del token pasado por parámetro.
 */
string get_token_name(token tok) {
    string name = tok_names[tok];
    return name != NULL ? name : "NIL";
}

/* Devuelve el token del nombre pasado por parámetro.
 */
token get_token_from_name(string name) {
    char NAME[32];
    strcpy(NAME, name);
    change_string_case(NAME, uppercase);
    int tok = find_string_in_array(NAME, tok_names);
    return tok >= 0 ? tok : NIL;
}

/* Devuelve el índice de la columna de la tabla de transiciones
 * correspondiente al carácter pasado como parámetro.
 */
short character_type_of(char c) {
    char cs[] = "();,:=+-*/\n";
    unsigned i, n = (unsigned) strlen(cs);
    cs[n] = EOF;
    
    for (i = 0; i <= n; i++)
        if (c == cs[i])
            return i;
    
    if (isalpha(c)) return i;
    if (isdigit(c)) return i + 1;
    if (isspace(c)) return i + 2;
    return i + 3;
}

/* Lee un carácter del archivo de entrada, lo guarda en el
 * buffer y lo devuelve.
 */
char read_char(void) {
    char c = fgetc(fin);
    lexeme[lexindex++] = c;
    lexeme[lexindex] = '\0';
    if (c == '\n') yyline++;
    return c;
}

/* Restaura el último carácter leído del archivo de entrada
 * y lo saca del buffer.
 */
void unread_char(void) {
    char c = lexeme[--lexindex];
    ungetc(c, fin);
    lexeme[lexindex] = '\0';
    if (c == '\n') yyline--;
}

token scanner(void) {
    unsigned ctype, state;
    state = lexindex = 0;
    
    while (state < 7) {
        char c = read_char();
        if (state == 0 && isspace(c))
            lexindex--;
        
        ctype = character_type_of(c);
        state = transition_table[state][ctype];
    }
    
    switch (state) {
        case  7: // reconoce identificador
            unread_char();
            return ID;
        case  8: // reconoce constante
            unread_char();
            return CONSTANTE;
        case  9: // reconoce paréntesis izquierdo [(]
            return PARENIZQUIERDO;
        case 10: // reconoce paréntesis derecho [)]
            return PARENDERECHO;
        case 11: // reconoce punto y coma (;)
            return PUNTOYCOMA;
        case 12: // reconoce coma (,)
            return COMA;
        case 13: // reconoce operador de asignación (:=)
            return ASIGNACION;
        case 14: // reconoce operador de adición (+)
            return SUMA;
        case 15: // reconoce operador de sustracción (-)
            return RESTA;
        case 16: // reconoce operador de multiplicación (*)
            return MULTIPLICACION;
        case 17: // reconoce operador de división (/)
            unread_char();
            return DIVISION;
        case 18: // reconoce comentario
            unread_char();
            return COMENTARIO;
        case 19: // reconoce fin de texto
            strcpy(lexeme, "(fdt)");
            return FDT;
        case 20: // reconoce errores léxicos
            return ERRORLEXICO;
        case 21: // reconoce errores de operador de asignación mal formado
            return ERRORASIG;
        case 22: // reconoce errores de constante mal formada
            unread_char();
            return ERRORCTE;
        default:
            strcpy(lexeme, "(nil)");
            return NIL;
    }
}

token next_token(void) {
    if (read_next_token) {
        read_next_token = false;
        do {
            current_token = scanner();
        } while (current_token == COMENTARIO);
        switch (current_token) {
            case ID:
                buscar(yytext, symbols, &current_token);
                break;
            case ERRORLEXICO:
            case ERRORASIG:
            case ERRORCTE:
                lexical_error(current_token);
            default: ;
        }
    }
    return current_token;
}

void match(token t) {
    if (next_token() != t) syntax_error(t, NULL);
    read_next_token = true;
}

/* Funciones auxiliares para probar el escáner y mostrarlo por pantalla */

#define COL1LENGTH 50
#define COL2LENGTH 16
#define COL3LENGTH 10
#define TEMPFILEPATH "/tmp/micro-scanner-input-file"

int column_lengths[] = {COL1LENGTH, COL2LENGTH, COL3LENGTH};

/* Imprime una barra divisoria de la tabla según el patrón indicado.
 */
void print_div(string pattern) {
    int i, j;
    for (i = 0; i < 7; i++) {
        if (i % 2)
            for (j = column_lengths[(i - 1) / 2]; j--;
                 fprintf(fout, "%s", wide_char_at(i, pattern)));
        else fprintf(fout, "%s", wide_char_at(i, pattern));
    }
    fputc('\n', fout);
}

/* Imprime una fila de la tabla con los valores pasados por parámetro.
 */
void print_row(string col1, string col2, string col3) {
    string cols[] = {col1, col2, col3}, sep = "┃│";
    for (int i = 0; i < 7; i++) {
        if (i % 2)
            fprintf(fout, " %-*s", column_lengths[(i - 1) / 2] - 1, cols[(i - 1) / 2]);
        else
            fprintf(fout, "%s", wide_char_at(i % 6 ? 1 : 0, sep));
    }
    fputc('\n', fout);
}

/* Atrapa la combinación de teclas Ctrl-C, imprime la última línea de la tabla,
 * cierra y elimina el archivo temporal, y termina la ejecución del programa.
 */
void handle_interrupt_signal() {
    putchar('\r');
    print_div("┗━┷━┷━┛");
    fclose(fin);
    remove(TEMPFILEPATH);
    exit(EXIT_SUCCESS);
}

/* Función para probar el escáner.
 */
void test_scanner(void) {
    print_div("┏━┯━┯━┓");
    print_row("LEXEME", "TOKEN", "LINE");
    print_div("┠─┼─┼─┨");
    
    token t = NIL;

    if (fin == stdin && fout == stdout) {
        signal(SIGINT, handle_interrupt_signal);
        fin = fopen(TEMPFILEPATH, "w+");
        int c, i;
        char input[COL1LENGTH];
        while (true) {
            printf("┃ ");
            while ((c = getchar()) != '\n') fputc(c, fin);
            rewind(fin);
            for (i = 0; (c = fgetc(fin)) != EOF; i++) input[i] = c;
            input[i] = '\0';
            
            printf("\033[A\033[2K");
            print_row(input, "(INPUT)", string_from_int(yyline));
            rewind(fin);
            while ((t = scanner()) != FDT)
                print_row(yytext, get_token_name(t), string_from_int(yyline));
            
            yyline++;
            rewind(fin);
            freopen(TEMPFILEPATH, "w+", fin);
        }
    } else {
        while ((t = scanner()) != FDT)
            print_row(yytext, get_token_name(t), string_from_int(yyline));
        print_div("┗━┷━┷━┛");
    }
}
