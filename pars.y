%union {
    int int_const;
    float float_const;
    string* string_const;
    Op_type op;
    Range* rang;
    Column_desc* col_desc;
    Update_pair* update_pair;
    vector<Table*>* vec_table;
    pair<string*, vector<string>*>* name_cols;
    Table* tabl;
    vector<string*>* vec_string;
    AST* ast;
    vector<Coumn_desc*>* vec_col_desc;
    vector<Update_pair*>* vec_update_pair;
}

%token <string_const> DOT_NAME NAME TEXT_CONSTANT INT_CONSTANT FLOAT_CONSTANT
%token <op> AND OR NOT MULT PLUS MINUS DIV GE LT GT LE NE EQ
%token SEMICOLON COMMIT ROLLBACK WITH COMMA AS ROUND_BRACKET_OPEN ROUND_BRACKET_CLOSE SELECT FROM WHERE CREATE TABLE INT FLOAT TEXT RANGE PRIMARY KEY INSERT INTO VALUES ASSIGN UPDATE SET DELETE INFINITY 

%left OR
%left AND
%right NOT
%nonassoc NE EQ LT LE GT GE
%left PLUS MINUS
%left MULT DIV

%type <vec_table> table_as_list
%type <name_cols> table_desc table_as
%type <tabl> select_query create_query insert_query update_query delete_query
%type <vec_string> column_list table_list constraint column_val_list
%type <string_const> column table column_val
%type <vec_col_desc> column_desc_list
%type <col_desc> column_desc
%type <rang> range
%type <update_pair> update
%type <vec_update_pair> update_list

%%

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
    : table_as COMMA table_as {$$ = new vector<Table*> (1, $1); $$->push_back($3);}
    | table_as {$$ = new vector<Table*> (1, $1);}
;

table_as
    : table_desc AS select_query {$$ = $3; $$->schema = $1->second; $$->name = $1->first;}
;

table_desc
    : NAME ROUND_BRACKET_OPEN column_list ROUND_BRACKET_CLOSE {$$ = new pair<string, vector<string>> ($1, $3);}
;

select_query
    : SELECT column_list FROM table_list {$$ = execute_select($4, $2);}
    | SELECT column_list FROM table_list WHERE condition {$$ = execute_select($4, $2, $6);}
;

column_list
    : column_list COMMA column {$$ = $1; $$->push_back($3);}
    | column {$$ = new vector<string> (1, $1);}
;

column
    : DOT_NAME {$$ = $1;}
    | NAME {$$ = $1;}
;

table_list
    : table COMMA table {$$ = $1; $$->push_back($3);}
    | table {$$ = new vector<string> (1, $1);}
;

table
    : NAME {$$ = $1;}
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
    : expression MULT expression {$$ = new AST($1, $3, $2);}
    | expression PLUS expression {$$ = new AST($1, $3, $2);}
    | expression MINUS expression {$$ = new AST($1, $3, $2);}
    | expression DIV expression {$$ = new AST($1, $3, $2);}
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
    : CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list COMMA constraint ROUND_BRACKET_CLOSE  {$$ = execute_create($3, $5, $7);}
    | CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list ROUND_BRACKET_CLOSE {$$ = execute_create($3, $5);}
;


column_desc_list
    : column_desc_list COMMA column_desc {$$->push_back($3);}
    | column_desc {$$ = new vector<col_desc> (1, $1);}
;

column_desc
    : NAME TYPE range {$$ = new col_desc($1, $2, $3);}
    | NAME TYPE {$$ = new col_desc($1, $2);}
;

range
    : RANGE ROUND_BRACKET_OPEN BETWEEN CONSTANT AND CONSTANT ROUND_BRACKET_CLOSE {$$ = new range($1, $2);}
;


constraint
    : PRIMARY KEY ROUND_BRACKET_OPEN column_list ROUND_BRACKET_CLOSE {$$ = $4;}
;

insert_query
    : INSERT INTO NAME VALUES ROUND_BRACKET_OPEN column_val_list ROUND_BRACKET_CLOSE {$$ = execute_insert($3, $6);}
;

column_val_list
    : column_val_list COMMA column_val {$$->push_back($3);}
    | column_val {$$ = new vector<string> (1, $1);}
;

column_val
    : CONSTANT {$$ = $1;}
;

update_query
    : UPDATE NAME SET update_list WHERE condition {$$ = execute_update($2, $4, $6);}
    | UPDATE NAME SET update_list {$$ = execute_update($2, $4);}
;

update_list
    : update_list COMMA update {$$->push_back($3);}
    | update {$$ = new vector<update_pair> (1, $1);}
;

update
    : NAME ASSIGN NAME {$$ = new update_pair($1, $3);}
    | NAME ASSIGN constant {$$ = new update_pair($1, $3);}
;           

delete_query
    : DELETE FROM NAME condition {$$ = execute_delete($3, $4);}
    | DELETE FROM NAME {$$ = execute_delete($3);}
;