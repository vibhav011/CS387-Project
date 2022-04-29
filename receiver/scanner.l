%option noinput nounput noyywrap 8bit                                 
%option yylineno
%option reentrant bison-bridge bison-locations 

%{
#include "query.h"
#include "commit.h"
#include "ast.h"
#include "parser.tab.h"

// void addtoken(string);
int lineno = 1;

%}

digit [0-9]
letter [a-zA-Z_]

%%

; { return SEMICOLON; }
(COMMIT|commit) { return COMMIT; }
(ROLLBACK|rollback) { return ROLLBACK; }
(WITH|with) { return WITH; }
, { return COMMA; }
(AS|as) { return AS; }
"(" { return ROUND_BRACKET_OPEN; }
")" { return ROUND_BRACKET_CLOSE; }
(SELECT|select) { return SELECT; }
(FROM|from) { return FROM; }
(WHERE|where) { return WHERE; }
(AND|and) { return AND; }
(OR|or) { return OR; }
("!"|NOT) { return NOT; }
"+" { return PLUS; }
"-" { return MINUS; }
"/" { return DIV; }
">=" { return GE; }
"<" { return LT; }
">" { return GT; }
"<=" { return LE; }
"!=" { return NE; }
"==" { return EQ; }
"*" { return STAR; }
"=" { return ASSIGN; }
(CREATE|create) { return CREATE; }
(BETWEEN|between) { return BETWEEN; }
(TABLE|table) { return TABLE; }

(INT|int) { 
    yylval->int_val = INT;
    return INTEGER; 
}

(DOUBLE|double) { 
    yylval->int_val = DOUBLE;
    return FLOAT; 
}

(VARCHAR|varchar) { 
    yylval->int_val = VARCHAR;
    return TEXT; 
}

(RANGE|range) { return RANGE; }
(PRIMARY|primary) { return PRIMARY; }
(KEY|key) { return KEY; }
(INSERT|insert) { return INSERT; }
(INTO|into) { return INTO; }
(VALUES|values) { return VALUES; }
(UPDATE|update) { return UPDATE; }
(SET|set) { return SET; }
(DELETE|delete) { return DELETE; }

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
    string s = string(yytext);
    yylval->str = new string(s.substr(1, s.size()-2));
    return TEXT_CONSTANT; 
}

[\-]?{digit}+ {
    yylval->str = new string(yytext);
    return INT_CONSTANT; 
}

{digit}+\.{digit}+ { 
    yylval->str = new string(yytext);
    return FLOAT_CONSTANT; 
}

. {}

%%