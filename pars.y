%{
#include "query.h"
#include "ast.h"

extern table_list results;
int const_type;
%}

%union {
    int int_val;
    string* str;
    Range* range;
    Constant* constant;
    Column_Desc* col_desc;
    Update_pair* update_pair;
    Temp_Table* table;
    pair<string, vector<string> >* name_cols;
    vector<Temp_Table*>* vec_table;
    vector<string>* vec_string;
    vector<Column_Desc*>* vec_col_desc;
    vector<Update_pair*>* vec_update_pair;
    ExprAST* exprast;
    CondAST* logast;
    RelAST* relast;
}

%token <str> DOT_NAME NAME TEXT_CONSTANT INT_CONSTANT FLOAT_CONSTANT
%token <int_val> INT FLOAT TEXT
%token AND OR NOT MULT PLUS MINUS DIV GE LT GT LE NE EQ
%token SEMICOLON COMMIT ROLLBACK WITH COMMA AS ROUND_BRACKET_OPEN ROUND_BRACKET_CLOSE SELECT FROM WHERE CREATE TABLE RANGE PRIMARY KEY INSERT INTO VALUES ASSIGN UPDATE SET DELETE INFINITY BETWEEN

%left OR
%left AND
%right NOT
%nonassoc NE EQ LT LE GT GE
%left PLUS MINUS
%left MULT DIV

%type <constant> constant
%type <vec_table> table_as_list
%type <table> table_as select_query create_query insert_query update_query delete_query
%type <name_cols> table_desc
%type <vec_string> column_list table_list, constraint column_val_list
%type <str> column table column_val
%type <vec_col_desc> column_desc_list
%type <col_desc> column_desc
%type <range> range
%type <update_pair> update
%type <vec_update_pair> update_list
%type <exprast> expression
%type <relast> relex
%type <logast> condition

%%

query
    : with_query SEMICOLON query SEMICOLON
    | select_query SEMICOLON
    {
        results[results_ind] = $1;
    }
    | create_query SEMICOLON
    | insert_query SEMICOLON
    | update_query SEMICOLON
    | delete_query SEMICOLON
    | COMMIT SEMICOLON
    | ROLLBACK SEMICOLON
;

with_query
    : WITH table_as_list 
    {
        checkerr(execute_create_temp(*($2)));
    }
;

table_as_list
    : table_as COMMA table_as 
    {
        $$ = new table_list(1, $1); 
        $$->push_back($3);
    }
    | table_as 
    {
        $$ = new table_list(1, $1);
    }
;

table_as
    : table_desc AS select_query 
    {
        $$ = $3; 
        $$->set_schema_columns($1->second);
        $$->set_name($1->first);
    }
;

table_desc
    : NAME ROUND_BRACKET_OPEN column_list ROUND_BRACKET_CLOSE 
    {
        $$ = new pair<string, vector<string> > (*$1, *$3);
    }
;

select_query
    : SELECT column_list FROM table_list 
    {
        Temp_table *temp = new Temp_Table();
        check_err(execute_select(temp, *$4, *$2));
        $$ = temp;
    }
    | SELECT column_list FROM table_list WHERE condition 
    {
        Temp_table *temp = new Temp_Table();
        check_err(execute_select(temp, *$4, *$2, $6));
        $$ = temp;
    }
;

column_list
    : column_list COMMA column 
    {
        $$ = $1;
        $$->push_back(*$3);
    }
    | column 
    {
        $$ = new vector<string> (1, *$1);
    }
;

column
    : DOT_NAME 
    | NAME 
;

table_list
    : table COMMA table 
    {
        $$ = new vector<string> (1, *$1);
        $$->push_back(*$3);
    }
    | table 
    {
        $$ = new vector<string> (1, *$1);
    }
;

table
    : NAME
;

condition
    : condition AND condition 
    {
        $$ = new BinLogAST($1, $3, _AND);
    } 
    | condition OR condition 
    {
        $$ = new BinLogAST($1, $3, _OR);
    }
    | NOT condition 
    {
        $$ = new UnaryLogAST($2);
    }
    | relex
;

relex
    : expression GE expression
    {
        $$ = new RelAST($1, $3, _GE);
    }
    | expression LT expression
    {
        $$ = new RelAST($1, $3, _LT);
    }
    | expression GT expression
    {
        $$ = new RelAST($1, $3, _GT);
    }
    | expression LE expression
    {
        $$ = new RelAST($1, $3, _LE);
    }
    | expression NE expression
    {
        $$ = new RelAST($1, $3, _NE);
    }
    | expression EQ expression
    {
        $$ = new RelAST($1, $3, _EQ);
    }
;

expression
    : expression MULT expression 
    {
        $$ = new ExprAST($1, $3, _MULT);
    }
    | expression PLUS expression
    {
        $$ = new ExprAST($1, $3, _PLUS);
    }
    | expression MINUS expression
    {
        $$ = new ExprAST($1, $3, _MINUS);
    }
    | expression DIV expression
    {
        $$ = new ExprAST($1, $3, _DIV);
    }
    | constant 
    {
        $$ = new ConstAST(*$1);
    }
    | DOT_NAME 
    {
        $$ = new ColAST(*$1);
    }
;

constant
    : TEXT_CONSTANT {$$ = new Constant(*$1, _TEXT);}
    | INT_CONSTANT {$$ = new Constant(*$1, _INT);}
    | FLOAT_CONSTANT {$$ = new Constant(*$1, _FLOAT);}
;

create_query
    : CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list COMMA constraint ROUND_BRACKET_CLOSE  
    {
        checkerr(execute_create(*$3, *$5, *$7));
    }
    | CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list ROUND_BRACKET_CLOSE 
    {
        checkerr(execute_create(*$3, *$5));
    }
;

column_desc_list
    : column_desc_list COMMA column_desc 
    {
        $$->push_back($3);
    }
    | column_desc 
    {
        $$ = new vector<col_desc> (1, $1);
    }
;

column_desc
    : NAME type range 
    {
        $$ = new col_desc(*$1, $2, $3->lower_bound, $3->upper_bound);
    }
    | NAME type 
    {
        $$ = new col_desc(*$1, $2);
    }
;

type 
    : INT
    | FLOAT
    | TEXT
;

range
    : RANGE ROUND_BRACKET_OPEN BETWEEN constant AND constant ROUND_BRACKET_CLOSE 
    {
        $$ = new Range(*$4, *$6);
    }
;

constraint
    : PRIMARY KEY ROUND_BRACKET_OPEN column_list ROUND_BRACKET_CLOSE {$$ = $4;}
;

insert_query
    : INSERT INTO NAME VALUES ROUND_BRACKET_OPEN column_val_list ROUND_BRACKET_CLOSE 
    {
        checkerr(execute_insert(*$3, *$6));
    }
;

column_val_list
    : column_val_list COMMA column_val 
    {
        $$->push_back(*$3);
    }
    | column_val 
    {
        $$ = new vector<string> (1, *$1);
    }
;

column_val
    : TEXT_CONSTANT
;

update_query
    : UPDATE NAME SET update_list WHERE condition 
    {
        checkerr(execute_update(*$2, *$4, $6));
    }
    | UPDATE NAME SET update_list 
    {
        checkerr(execute_update($2, $4));
    }
;

update_list
    : update_list COMMA update {$$->push_back($3);}
    | update {$$ = new vector<update_pair*> (1, $1);}
;

update
    : NAME ASSIGN NAME {$$ = new update_pair($1, $3);}
    | NAME ASSIGN constant {$$ = new update_pair($1, $3);}
;           

delete_query
    : DELETE FROM NAME condition 
    {
        checkerr(execute_delete($3, $4));
    }
    | DELETE FROM NAME 
    {
        checkerr(execute_delete($3));
    }
;