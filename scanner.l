%option noinput nounput noyywrap 8bit                                 
%option yylineno
%option reentrant bison-bridge bison-locations 

%{
#include "./receiver/query.h"
#include "ast.h"
#include "parser.tab.h"

// void addtoken(string);
int lineno = 1;

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
"(" { cout<<"left\n";return ROUND_BRACKET_OPEN; }
")" { cout<<"right\n";return ROUND_BRACKET_CLOSE; }
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
"*" { return STAR; }
CREATE { return CREATE; }
BETWEEN { return BETWEEN; }
TABLE { return TABLE; }

INT { 
    yylval->int_val = INT;
    return INTEGER; 
}

DOUBLE { 
    yylval->int_val = DOUBLE;
    return FLOAT; 
}

VARCHAR { 
    yylval->int_val = VARCHAR;
    return TEXT; 
}

RANGE { return RANGE; }
PRIMARY { return PRIMARY; }
KEY { return KEY; }
INSERT { return INSERT; }
INTO { return INTO; }
VALUES { return VALUES; }
"=" { return ASSIGN; }
UPDATE { return UPDATE; }
SET { return SET; }
DELETE { return DELETE; }
INFINITY { return INFINITY; }

{letter}({letter}|{digit})*\.{letter}({letter}|{digit})* { 
    yylval->str = new string();
    char txt[30];
    strcpy(txt, yytext);
    *(yylval->str) = txt;
    return DOT_NAME; 
}

{letter}({letter}|{digit})* {
    char txt[30];
    yylval->str = new string();
    strcpy(txt, yytext);
    *(yylval->str) = txt;
    return NAME; 
}

\"[^\"]+\" { 
    char txt[30];
    strcpy(txt, yytext);
    yylval->str = new string();
    *(yylval->str) = txt;
    return TEXT_CONSTANT; 
}

{digit}+ {
    char txt[30];
    yylval->str = new string();
    strcpy(txt, yytext);
    *(yylval->str) = txt;
    return INT_CONSTANT; 
}

{digit}+\.{digit}+ { 
    char txt[30];
    yylval->str = new string();
    strcpy(txt, yytext);
    *(yylval->str) = txt;
    return FLOAT_CONSTANT; 
}

. {}

%%