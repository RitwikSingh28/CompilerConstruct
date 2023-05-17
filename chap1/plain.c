#include <stdio.h>
#include "lex.h"

statements() {
    // statements -> expression SEMI | expression SEMI statements
    expression();
    if(match(SEMI)) advance();
    else fprintf(stderr, "%d: expected semi-colon.\n", yylineno);

    if(!match(EOI)) statements();
}

expression() {
    // expression -> term expr_prime
    term();
    expr_prime();
}

expr_prime() {
    // expr_prime -> PLUS term expr_prime | epsilon
    if(match(PLUS)) {
        advance();
        term();
        expr_prime();
    }
}

term() {
    // term -> factor term_prime
    factor();
    term_prime();
}

term_prime() {
    // term_prime -> MUL factor term_prime | epsilon
    if(match(MUL)) {
        factor();
        term_prime();
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