/*
 *  main.c
 *  Inicialización del programa
 */

#include "parser.h"
#include "error.h"
#include "symbol.h"

int main(int argc, string argv[]) {
    fin = stdin;
    fout = stdout;
    if (argc > 1) fin = fopen(argv[1], "r");
    if (argc > 2) fout = fopen(argv[2], "w");
    if(fin == NULL || ferror(fin)) error_de_archivo(argv[1]);
    if(fout == NULL || ferror(fout)) error_de_archivo(argv[2]);
    
    build_symbol_table();
    
    objetivo();
    
    fclose(fin);
    if (argc > 2) fclose(fout);
    return EXIT_SUCCESS;
}
