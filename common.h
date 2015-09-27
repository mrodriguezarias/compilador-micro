/*
 *  common.h
 *  Funciones auxiliares compartidas
 */

#ifndef common_h
#define common_h

#include <stdio.h> // FILE, fprintf(), fgetc(), ungetc(), etc.
#include <ctype.h> // isalpha(), isdigit(), isspace(), toupper(), tolower()
#include <string.h> // strcmp(), strcpy(), strlen()
#include <stdlib.h> // exit(), EXIT_SUCCESS
#include <signal.h> // signal()
#include <stdbool.h> // bool, true, false

typedef enum {lowercase, uppercase} case_type;
void change_string_case(char * const, case_type);

bool strings_are_equal(const char *, const char *);

int find_string_in_array(const char *, const char **);

const char * string_from_int(int);

const char * wide_char_at(int, const char *);

#endif /* common_h */
