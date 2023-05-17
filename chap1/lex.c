#include "lex.h"
#include <stdio.h>
#include <ctype.h>

char *yytext = "";
int yylen = 0;
int yylineno = 0;

lex() {
    //create a 128 B buffer to read an input line at a time
    char buffer[128];
    char* current;      //pointer to the current character to be read

    current = yytext + yylen;   // skip current lexeme

    while(1) {
        while(!*current) {
            current = buffer;
            if(!gets(buffer)) {
                *current = '\0';
                return EOI;
            }

            ++yylineno;
            while(isspace(*current)) ++current;
        }

        for(;*current; ++current) {
            //Get the next token

            yytext = current;
            yylen = 1;

            switch (*current)
            {
            case EOF: return EOI;
            case ';': return SEMI;
            case '+': return PLUS;
            case '*': return MUL;
            case '(': return LP;
            case ')': return RP;

            //skip the blankspaces
            case '\n':
            case '\t':
            case ' ': break; 
            
            default:
                if(!isalnum(*current)){
                    fprintf(stderr, "Ignoring illegal input <%c>\n", *current);
                }
                else {
                    while (isalnum(*current)) ++current;

                    yylen = current - yytext;
                    return NUM_OR_ID;
                }
                break;
            }
        }
    }
}

static int Lookahead = -1;

int match(int token) {
    if(Lookahead == -1) {
        Lookahead = lex();
    }

    return token == Lookahead;
}

void advance() {
    //Advance the lookahead to the next input symbol
    Lookahead = lex();
}