%{
void yyerror(string);
void addtoken(string);
int lineno = 1;
#include "y.tab.h"
%}

digit [0-9]
letter [a-zA-Z]

%%

; { return SEMICOLON; }
COMMIT { return COMMIT; }
ROLLBACK { return ROLLBACK; }
WITH { return WITH; }
, { return COMMA; }
AS { return AS; }
( { return ROUND_BRACKET_OPEN; }
) { return ROUND_BRACKET_CLOSE; }
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
DELETE { return DELETETOK; }
INFINITY { return INFINITY; }

{letter}({letter})|({digit})*\.{letter}({letter})|({digit})* { 
    yylval = new string();
    *yylval.str = yytext;
    return DOT_NAME; 
}

{letter}({letter})|({digit})* {
    yylval = new string();
    *yylval.str = yytext;
    return NAME; 
}

\"[^\"]\" { 
    yylval.str = yytext;
    return TEXT_CONSTANT; 
}

{digit}+ {
    yylval = new string()
    *yylval.str = yytext; 
    return INT_CONSTANT; 
}

{digit}+\.{digit}+ { 
    yylval = new string()
    *yylval.str = yytext;
    return FLOAT_CONSTANT; 
}

%%