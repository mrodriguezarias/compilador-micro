/*
 *  main.c
 *  Inicialización del programa
 */

const float version_number = 1.0;

#include "parser.h"
#include "error.h"
#include "symbol.h"

FILE * open_file(string name, string mode) {
    FILE * file = fopen(name, mode);
    if (file == NULL || ferror(file))
        file_error(name);
    return file;
}

void display_help(void) {
    puts("Uso: mlc [opciones] [modo] [-i] archivo-entrada [-o] archivo-salida [-e] archivo-error\n");
    
    puts("Opciones:");
    puts("-h\t: muestra este mensaje de ayuda y termina la ejecución del programa.");
    puts("-v\t: imprime el número de versión del programa y termina la ejecución.\n");
    
    puts("Modos de compilación (valor por defecto: -a)");
    puts("-a\t: analiza léxica, sintáctica y semánticamente el archivo de entrada.");
    puts("-p\t: analiza léxica y sintácticamente el archivo de entrada.");
    puts("-s\t: analiza léxicamente el archivo de entrada.\n");
    
    puts("Archivos:");
    puts("-i\t: archivo de entrada. De no especificarse se usa la entrada estándar.");
    puts("-o\t: archivo de salida. De no especificarse se usa la salida estándar.");
    puts("-e\t: archivo de errores. De no especificarse se usa la salida de errores estándar.");
    puts("(Si los archivos se colocan en este orden no hace falta poner las opciones.)");
    
    exit(EXIT_SUCCESS);
}

void print_version(void) {
    printf("Micro Language Compiler v%.1f\n", version_number);
    exit(EXIT_SUCCESS);
}

/* micro [-s/-p/-a] [-i] "input-file" [-o] "output-file" [-e] "error-file"
 */
int main(int argc, string argv[]) {
    fin = stdin;
    fout = stdout;
    ferr = stderr;
    
    string file_ops = "ioe";
    string mode_ops = "spa";
    bool subarg = false;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (subarg) {
            subarg = false;
            switch (*(argv[i-1]+1)) {
                case 'i': fin = open_file(arg, "r"); break;
                case 'o': fout = open_file(arg, "w"); break;
                case 'e': ferr = open_file(arg, "w"); break;
                default:;
            }
        } else if (*arg == '-' && strlen(arg) == 2) {
            int mode, opt = *(arg+1);
            if (find_char_in_string(opt, file_ops) != -1)
                subarg = true;
            else if ((mode = find_char_in_string(opt, mode_ops)) != -1)
                compiler_mode = mode;
            else if (opt == 'h') display_help();
            else if (opt == 'v') print_version();
            else invalid_argument(arg);
        } else {
            if (fin == stdin) fin = open_file(arg, "r");
            else if (fout == stdout) fout = open_file(arg, "w");
            else if (ferr == stderr) ferr = open_file(arg, "w");
        }
    }
    if (subarg == true)
        invalid_argument(NULL);
    
    build_symbol_table();
    
    if (compiler_mode == SCAN)
        test_scanner();
    else
        objetivo();
    
    if (fin != stdin) fclose(fin);
    if (fout != stdout) fclose(fout);
    if (ferr != stderr) fclose(ferr);
    return EXIT_SUCCESS;
}
