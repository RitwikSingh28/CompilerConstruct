#include <stdio.h>
#include <stdarg.h>
#include "lex.h"

#define MAXFIRST 16
#define SYNCH SEMI

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

    if(!legal_lookahead(NUM_OR_ID, LP, 0)) return;

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
    if(!legal_lookahead(NUM_OR_ID, LP, 0)) return;
    factor();

    while(match(MUL)) {
        advance();
        factor();
    }
}

factor() {
    // factor -> NUM_OR_ID | LP expression RP
    if(!legal_lookahead(NUM_OR_ID, LP, 0)) return;

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

// Implementation of legal_lookahead, which checks if the lookahead symbol belongs to the FIRST 
// of a particular Non-terminal or not
legal_lookahead(int first_arg, ...) {
    va_list args;
    int tok;
    int lookaheads[MAXFIRST], *p = lookaheads, *current;
    int error_printed = 0;
    int rval = 0;   // indicates whether the error was recovered (1) or not (0)

    va_start(args, first_arg);

    if(!first_arg) {
        if(match(EOI)) rval = 1;
    }    
    else {
        *p++ = first_arg;
        while( (tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST] ) {
            *++p = tok;
        }

        /*
            The following loop is intended to discard input symbols until one that 
            matches the synchronization token ('SYNCH') is found
        */
        while(!match(SYNCH)) {
            // The parser has encountered a syntax error and needs to recover
            for(current = lookaheads; current < p; ++current) {
                if(match(*current)) {
                    rval = 1;   //the error has been successfully recovered
                    goto exit;
                }
            }

            if(!error_printed) {
                fprintf(stderr, "Line %d: Syntax error.\n", yylineno);
                error_printed = 1;
            }

            advance();
        }
    }

    exit:
        va_end(args);
        return rval;
}