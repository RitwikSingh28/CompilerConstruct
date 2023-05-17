#include <stdio.h>
#include "lex.h"

statements() {
    // statements -> expression SEMI | expression SEMI statements
    expression();
    if(match(SEMI)) advance();
    else fprintf(stderr, "%d: expected semi-colon.\n", yylineno);

    if(!match(EOI)) statements();
}

/*
    This function has been improvised by removing the unnecessary tail recursion at the end
    The tail recursion has been replaced by a loop instead
*/
expression() {
    // expression -> term expr_prime
    // expr_prime -> PLUS term expr_prime | epsilon
    term();

    while(match(PLUS)) {
        advance();
        term();
    }
}

/*
    Similar optimization here
*/
term() {
    // term -> factor term_prime
    // term_prime -> MUL factor term_prime | epsilon
    factor();

    while(match(MUL)) {
        advance();
        factor();
    }
}

factor() {
    // factor -> NUM_OR_ID | LP expression RP
    if(match(NUM_OR_ID)) advance();
    else if(match(LP)) {
        advance();
        expression();
        if(match(RP)) advance();
        else fprintf(stderr, "%d: mismatched parantheses.\n", yylineno);
    }
    else {
        fprintf(stderr, "%d: expected an integer or an identifier.\n", yylineno);
    }
}