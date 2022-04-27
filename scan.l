%{
#include "ast.h"
#include "utils.h"
#include "./receiver/query.h"
#include "y.tab.h"
extern void yyerror(int, string);
void addtoken(string);
int lineno = 1;

int readInputForLexer( char *buffer, unsigned long *numBytesRead, int maxBytesToRead);
#undef YY_INPUT
#define YY_INPUT(b,r,s) readInputForLexer(b,&r,s)
%}

digit [0-9]
letter [a-zA-Z_]

%%

; { return SEMICOLON; }
COMMIT { return COMMIT; }
ROLLBACK { return ROLLBACK; }
WITH { return WITH; }
, { return COMMA; }
AS { return AS; }
"(" { return ROUND_BRACKET_OPEN; }
")" { return ROUND_BRACKET_CLOSE; }
SELECT { return SELECT; }
FROM { return FROM; }
WHERE { return WHERE; }
AND { return AND; }
OR { return OR; }
NOT { return NOT; }
MULT { return MULT; }
PLUS { return PLUS; }
MINUS { return MINUS; }
DIV { return DIV; }
">=" { return GE; }
"<" { return LT; }
">" { return GT; }
"<=" { return LE; }
"!=" { return NE; }
"==" { return EQ; }
CREATE { return CREATE; }
BETWEEN { return BETWEEN; }
TABLE { return TABLE; }

INT { 
    yylval.int_val = _INT;
    return INTTOK; 
}

FLOAT { 
    yylval.int_val = _FLOAT;
    return FLOAT; 
}

TEXT { 
    yylval.int_val = _TEXT;
    return TEXT; 
}

RANGE { return RANGE; }
PRIMARY { return PRIMARY; }
KEY { return KEY; }
INSERT { return INSERTTOK; }
INTO { return INTO; }
VALUES { return VALUES; }
"=" { return ASSIGN; }
UPDATE { return UPDATETOK; }
SET { return SET; }
DELETE { return DELETE; }
INFINITY { return INFINITY; }

{letter}({letter}|{digit})*\.{letter}({letter}|{digit})* { 
    yylval.str = new string();
    *yylval.str = yytext;
    return DOT_NAME; 
}

{letter}({letter}|{digit})* {
    yylval.str = new string;
    *yylval.str = yytext;
    return NAME; 
}

\"[^\"]\" { 
    *yylval.str = yytext;
    return TEXT_CONSTANT; 
}

{digit}+ {
    yylval.str = new string;
    *yylval.str = yytext; 
    return INT_CONSTANT; 
}

{digit}+\.{digit}+ { 
    yylval.str = new string;
    *yylval.str = yytext;
    return FLOAT_CONSTANT; 
}

%%

int readInputForLexer( char *buffer, unsigned long *numBytesRead, int maxBytesToRead ) {
    unsigned long numBytesToRead = maxBytesToRead;
    int bytesRemaining = global_query.size()-globalReadOffset;
    int i;
    if ( numBytesToRead > bytesRemaining ) { numBytesToRead = bytesRemaining; }
    for ( i = 0; i < numBytesToRead; i++ ) {
        buffer[i] = global_query[globalReadOffset+i];
    }
    *numBytesRead = numBytesToRead;
    globalReadOffset += numBytesToRead;
    return 0;
}