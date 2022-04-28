%define api.pure full
%locations
%param { yyscan_t scanner }

%{
#include "./receiver/query.h"
#include "ast.h"
#include "utils.h"

extern vector<Temp_Table*> results;
int const_type;

%}

%code requires {
  typedef void* yyscan_t;
}

%code {
    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg);
    void* yyget_extra ( yyscan_t yyscanner );
    void checkerr(int err_code);
}

%union {
    int int_val;
    string* str;
    Range* range;
    Constant* constant;
    ColumnDesc* col_desc;
    Update_Pair* update_pair;
    Temp_Table* table;
    pair<string, vector<string> >* name_cols;
    vector<Temp_Table*>* vec_table;
    vector<string>* vec_string;
    vector<ColumnDesc*>* vec_col_desc;
    vector<Update_Pair*>* vec_update_pair;
    ExprAST* exprast;
    CondAST* logast;
    RelAST* relast;
}

%token <str> DOT_NAME NAME TEXT_CONSTANT INT_CONSTANT FLOAT_CONSTANT
%token <int_val> INTEGER FLOAT TEXT
%token AND OR NOT MULT PLUS MINUS DIV GE LT GT LE NE EQ
%token SEMICOLON COMMIT ROLLBACK WITH COMMA AS ROUND_BRACKET_OPEN ROUND_BRACKET_CLOSE SELECT FROM WHERE CREATE TABLE RANGE PRIMARY KEY INSERT INTO VALUES ASSIGN UPDATE SET DELETE INFINITY BETWEEN

%left OR
%left AND
%right NOT
%nonassoc NE EQ LT LE GT GE
%left PLUS MINUS
%left MULT DIV
%nonassoc REDC
%nonassoc COMMA

%type <int_val> type
%type <constant> constant
%type <vec_table> table_as_list
%type <table> table_as select_query create_query insert_query update_query delete_query
%type <name_cols> table_desc
%type <vec_string> column_list table_list constraint column_val_list
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
        cout << "here" << endl;
        cout << *(int *)yyget_extra(scanner) << endl;
        results[*(int *)yyget_extra(scanner)] = $1;
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
        Temp_Table *temp = new Temp_Table();
        checkerr(execute_select(temp, *$4, *$2));
        $$ = temp;
    }
    | SELECT column_list FROM table_list WHERE condition 
    {
        Temp_Table *temp = new Temp_Table();
        checkerr(execute_select(temp, *$4, *$2, $6));
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
        cout << "here" << endl;
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
    | table %prec REDC
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
    {
        $$ = (CondAST *)$1;
    }
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
        $$ = new BinArithAST($1, $3, _MULT);
    }
    | expression PLUS expression
    {
        $$ = new BinArithAST($1, $3, _PLUS);
    }
    | expression MINUS expression
    {
        $$ = new BinArithAST($1, $3, _MINUS);
    }
    | expression DIV expression
    {
        $$ = new BinArithAST($1, $3, _DIV);
    }
    | constant 
    {
        $$ = new ConstAST($1);
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
        $$ = new vector<ColumnDesc*> (1, $1);
    }
;

column_desc
    : NAME type range 
    {
        if($2 == _TEXT)
            return -1;
        $$ = new ColumnDesc(&(*$1)[0], $2, $3->lower_bound, $3->upper_bound);
    }
    | NAME type 
    {
        $$ = new ColumnDesc(&(*$1)[0], $2);
    }
;

type 
    : INTEGER
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
        checkerr(execute_update(*$2, *$4));
    }
;

update_list
    : update_list COMMA update 
    {
        $$->push_back($3);
    }
    | update 
    {
        $$ = new vector<Update_Pair*> (1, $1);
    }
;

update
    : NAME ASSIGN NAME 
    {
        $$ = new Update_Pair(*$1, *$3);
    }
    | NAME ASSIGN constant 
    {
        $$ = new Update_Pair(*$1, $3->val);
    }
;           

delete_query
    : DELETE FROM NAME condition 
    {
        checkerr(execute_delete(*$3, $4));
    }
    | DELETE FROM NAME 
    {
        checkerr(execute_delete(*$3));
    }
;

%%

void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg)
{
    // cerr<<err<<endl;
    return;
}

void checkerr(int err_code) {
    switch(err_code) {
        case C_OK:
            cout<<"successfully terminated"<<endl;
            break;
        case C_TRUE:
            cout<<"true output"<<endl;
            break;
        case C_FALSE:
            cout<<"false output"<<endl;
            break;
        case C_ERROR:
            cout<<"error"<<endl;
            break;
        case C_TABLE_NOT_FOUND:
            cout<<"table not found"<<endl;
            break;
        case C_FIELD_NOT_FOUND:
            cout<<"field not found"<<endl;
            break;
    }
}