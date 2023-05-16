#define EOI         0   //End of Input Marker
#define SEMI        1   // ;
#define PLUS        2   // +
#define MUL         3   // *
#define LP          4   // (
#define RP          5   // )
#define NUM_OR_ID   6   // integer or identifier

extern char* yytext;    // pointer to current lexeme
extern int yylen;       // number of characters in the current lexeme
extern int yylineno;    // current input line number