#include <stdio.h>
#include "lex.h"

extern char* newName(void);
extern void freeName(char *name);

char *factor(void);
char *term(void);
char *expression(void);

statements() {
    // statements -> expression SEMI | expression SEMI statements 

    char *tempvar;

    while(!match(EOI)) {
        tempvar = expression();
        if(match(SEMI)) advance();
        else fprintf(stderr, "%d: semi-colon expected\n", yylineno);
        freename(tempvar);
    }
}

char* expression() {
    /*
        expression -> term expression'
        expression' -> PLUS term expression' | epsilon
    */ 
    char* tempvar1, tempvar2;

    tempvar1 = term();
    while(match(PLUS)) {
        advance();
        tempvar2 = term();
        printf("    %s += %s\n", tempvar1, tempvar2);
        freename(tempvar2);
    }
    return tempvar1;
}

char* term() {
    // term -> factor term_prime
    // term_prime -> MUL factor term_prime | epsilon
    char* tempvar1, tempvar2;
    tempvar1 = factor();

    while(match(MUL)) {
        advance();
        tempvar2 = factor();
        printf("    %s *= %s\n", tempvar1, tempvar2);
        freename(tempvar2);
    }
    return tempvar1;
}

char* factor() {
    // factor -> NUM_OR_ID | LP expression RP
    char* tempvar;
    if(match(NUM_OR_ID)) {
        printf("    %s = %0.*s\n", tempvar = newname(), yylen, yytext);
        advance();
    }
    else if(match(LP)) {
        advance();
        tempvar = expression();
        if(match(RP)) advance();
        else fprintf(stderr, "%d: Mismatched parantheses\n", yylineno);
    }
    else 
        fprintf(stderr, "%d: Number or identifier expected\n", yylineno);
    return tempvar;
}

