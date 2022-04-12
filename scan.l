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
TABLE { return TABLE; }
INT { return INT; }
FLOAT { return FLOAT; }
TEXT { return TEXT; }
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
{letter}{letter|digit}*\.{letter}{letter|digit}* { return DOT_NAME; }
{letter}{letter|digit}* { return NAME; }
\"[^\"]\" { return TEXT_CONSTANT; }
{digit}+ { return INT_CONSTANT; }
{digit}+\.{digit}+ { return FLOAT_CONSTANT; }