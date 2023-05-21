/*
    This C program contains a stack of temporary variables used for code generation in compilers
    newName() allocates a temporary variable for a subexpression
    freeName() frees the allocated temporary variable after its use so it can be recycled later
*/

#include <stdio.h>
#include "lex.h"

char *Names[] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"};
char **Namep = Names;       // top pointer to stack of Names

char* newName() {
    if( Namep > &Names[sizeof(Names)/sizeof(*Names)]) {     // Not enough temporary variables to assign to the expression
        fprintf(stderr, "%d: expression too complex\n", yylineno);
        exit(1);
    }
    return *++Namep;
}

void freeName(char *s) {
    if(*Namep > Names) {
        *--Namep = s;
    }
    else fprintf(stderr, "%d: (Internal error) Name stack underflow\n", yylineno);
}