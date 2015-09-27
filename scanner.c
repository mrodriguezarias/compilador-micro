/*
 *  scanner.c
 *  Implementación del escáner
 */

#include "scanner.h"

const char * tok_names[] = {
    "INICIO", "FIN", "LEER", "ESCRIBIR", "ID", "CONSTANTE",
    "PARENIZQUIERDO", "PARENDERECHO", "PUNTOYCOMA", "COMA",
    "ASIGNACION", "SUMA", "RESTA", "MULTIPLICACION", "DIVISION",
    "COMENTARIO", "FDT", "ERRORLEXICO", "ERRORASIG", "ERRORCTE", NULL
};

const char * reserved_keywords[] = {"inicio", "fin", "leer", "escribir", NULL};

static char lexeme[100];
char * const yytext = lexeme;
unsigned yyline = 1, lexindex = 0;
FILE * fin, * fout;

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
const char * get_token_name(token tok) {
    return tok_names[tok];
}

/* Devuelve el token del nombre pasado por parámetro.
 */
token get_token_from_name(const char * name) {
    char NAME[32];
    strcpy(NAME, name);
    change_string_case(NAME, uppercase);
    int tok = find_string_in_array(NAME, tok_names);
    return tok >= 0 ? tok : NIL;
}

/* Devuelve el índice de la columna de la tabla de transiciones
 * correspondiente al carácter pasado como parámetro.
 */
short get_symbol(char c) {
    char symbols[] = "();,:=+-*/\n";
    unsigned i, n = (unsigned) strlen(symbols);
    symbols[n] = EOF;
    
    for (i = 0; i <= n; i++)
        if (c == symbols[i])
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
    unsigned symbol, state;
    state = lexindex = 0;
    
    while (state < 7) {
        char c = read_char();
        if (state == 0 && isspace(c))
            lexindex--;
        
        symbol = get_symbol(c);
        state = transition_table[state][symbol];
    }
    
    switch (state) {
        case  7: // reconoce identificador
            unread_char();
            return find_string_in_array(lexeme, reserved_keywords) >= 0 ?
                get_token_from_name(lexeme) : ID;
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
            return DIVISION;
        case 18: // reconoce comentario
            unread_char();
            return COMENTARIO;
        case 19: // reconoce fin de texto
            yyline = 1;
            return FDT;
        case 20: // reconoce errores léxicos
            return ERRORLEXICO;
        case 21: // reconoce errores de operador de asignación mal formado
            return ERRORASIG;
        case 22: // reconoce errores de constante mal formada
            unread_char();
            return ERRORCTE;
        default:
            return NIL;
    }
}

/* Funciones auxiliares para probar el escáner y mostrarlo por pantalla */

#define COL1LENGTH 50
#define COL2LENGTH 16
#define COL3LENGTH 10
#define TEMPFILEPATH "/tmp/micro-scanner-input-file"

int column_lengths[] = {COL1LENGTH, COL2LENGTH, COL3LENGTH};

/* Imprime una barra divisoria de la tabla según el patrón indicado.
 */
void print_div(const char * pattern) {
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
void print_row(const char * col1, const char * col2, const char * col3) {
    const char * cols[] = {col1, col2, col3}, * sep = "┃│";
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

int main(int argc, const char * argv[]) {
    fin = stdin;
    fout = stdout;
    if (argc > 1) fin = fopen(argv[1], "r");
    if (argc > 2) fout = fopen(argv[2], "w");
    
    print_div("┏━┯━┯━┓");
    print_row("LEXEME", "TOKEN", "LINE");
    print_div("┠─┼─┼─┨");
    
    token t = NIL;

    if (argc == 1) {
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

    if (argc > 1) fclose(fin);
    if (argc > 2) fclose(fout);
    return EXIT_SUCCESS;
}
