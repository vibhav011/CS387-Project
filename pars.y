query
    : with_query SEMICOLON query SEMICOLON
    | select_query SEMICOLON
    | create_query SEMICOLON
    | insert_query SEMICOLON
    | update_query SEMICOLON
    | delete_query SEMICOLON
    | COMMIT SEMICOLON
    | ROLLBACK SEMICOLON
;

with_query
    : WITH table_as_list
;

table_as_list
    : table_as_list COMMA table_as
    | table_as
;

table_as
    : table_desc AS select_query
;

table_desc
    : NAME ROUND_BRACKET_OPEN column_list ROUND_BRACKET_CLOSE
;

select_query
    : SELECT column_list FROM table_list
    | SELECT column_list FROM table_list WHERE condition
;

column_list
    : column_list COMMA column
    | column
;

column
    : DOT_NAME
    | NAME
;

table_list
    : table COMMA table
    | table
;

table
    : NAME
;

condition
    : condition bin_boolop condition
    | un_boolop condition
    | relex bin_boolop relex
    | un_boolop relex
;

bin_boolop
    : AND
    | OR
;

un_relop
    : NOT
;

relex
    : expression relop expression
;

expression
    : expression MULT expression
    | expression PLUS expression
    | expression MINUS expression
    | expression DIV expression
    | constant
    | DOT_NAME
;


relop
    : GE
    | LT
    | GT
    | LE
    | NE
    | EQ
;

constant
    : TEXT_CONSTANT
    | INT_CONSTANT
    | FLOAT_CONSTANT
;

create_query
    : CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list COMMA constraint_list ROUND_BRACKET_CLOSE
    | CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list ROUND_BRACKET_CLOSE
;


column_desc_list
    : column_desc_list COMMA column_desc
    | column_desc
;

column_desc
    : NAME TYPE range
    | NAME TYPE
;

range
    : RANGE ROUND_BRACKET_OPEN CONSTANT AND CONSTANT ROUND_BRACKET_CLOSE
;

constraint_list
    : constraint_list COMMA constraint
    | constraint
;

constraint
    : PRIMARY KEY ROUND_BRACKET_OPEN column_list ROUND_BRACKET_CLOSE
;

insert_query
    : INSERT INTO NAME VALUES ROUND_BRACKET_OPEN column_val_list ROUND_BRACKET_CLOSE
;

column_val_list
    : column_val_list COMMA column_val
    | column_val
;

column_val
    : CONSTANT
;

update_query
    : UPDATE NAME SET update_list WHERE condition
    | UPDATE NAME SET update_list
;

update_list
    : update_list COMMA update
    | update
;

update
    : NAME ASSIGN NAME
    | NAME ASSIGN constant
;           

delete_query
    : DELETE FROM NAME condition
    | DELETE FROM NAME
;