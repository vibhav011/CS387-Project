%define api.pure full
%locations
%param { yyscan_t scanner }

%{
#include "./receiver/query.h"
#include "./receiver/commit.h"
#include "ast.h"
#include "utils.h"
#include "sock.hpp"

extern vector<Temp_Table*> results;
extern int obtain_write_lock(int worker_id, string table_names);
extern mutex create_mutex;
extern map<string, int> table_access;
vector<string> changed_tables[MAX_PROCESSES];
bool txn_active = false;

%}

%code requires {
  typedef void* yyscan_t;
}

%code {
    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg);
    void* yyget_extra ( yyscan_t yyscanner );
    void checkerr(int err_code, yyscan_t scanner);
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
%token AND OR NOT PLUS MINUS DIV GE LT GT LE NE EQ STAR
%token SEMICOLON COMMIT ROLLBACK WITH COMMA AS ROUND_BRACKET_OPEN ROUND_BRACKET_CLOSE SELECT FROM WHERE CREATE TABLE RANGE PRIMARY KEY INSERT INTO VALUES ASSIGN UPDATE SET DELETE BETWEEN

%left OR
%left AND
%right NOT
%nonassoc NE EQ LT LE GT GE
%left PLUS MINUS
%left STAR DIV
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
        results[((Pro *)yyget_extra(scanner))->id] = $1;
        txn_active = true;
    }
    | create_query SEMICOLON
    | insert_query SEMICOLON
    {
        txn_active = true;
    }
    | update_query SEMICOLON
    {
        txn_active = true;
    }
    | delete_query SEMICOLON
    {
        txn_active = true;
    }
    | COMMIT SEMICOLON
    {
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(execute_commit(changed_tables[worker_id]), scanner);
        changed_tables[worker_id].clear();
        txn_active = false;
    }
    | ROLLBACK SEMICOLON
    {
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(execute_rollback(changed_tables[worker_id]), scanner);
        changed_tables[worker_id].clear();
        txn_active = false;
    }
;

with_query
    : WITH table_as_list 
    {
        checkerr(execute_create_temp(*($2)), scanner);
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
    : SELECT STAR FROM table_list
    {
        Temp_Table *temp = new Temp_Table();
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(execute_select(temp, *$4, {"*"}, NULL, NULL, worker_id), scanner);
        $$ = temp;
    }
    | SELECT STAR FROM table_list WHERE condition
    {
        Temp_Table *temp = new Temp_Table();
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(execute_select(temp, *$4, {"*"}, $6, NULL, worker_id), scanner);
        $$ = temp;
    }
    | SELECT column_list FROM table_list 
    {
        Temp_Table *temp = new Temp_Table();
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(execute_select(temp, *$4, *$2, NULL, NULL, worker_id), scanner);
        $$ = temp;
    }
    | SELECT column_list FROM table_list WHERE condition 
    {
        Temp_Table *temp = new Temp_Table();
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(execute_select(temp, *$4, *$2, $6, NULL, worker_id), scanner);
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
        $$ = new RelAST($1, $3, __NE);
    }
    | expression EQ expression
    {
        $$ = new RelAST($1, $3, _EQ);
    }
;

expression
    : expression STAR expression 
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
    | NAME
    {   
        $$ = new ColAST(*$1);
    }
;

constant
    : TEXT_CONSTANT {$$ = new Constant(*$1, VARCHAR);}
    | INT_CONSTANT {$$ = new Constant(*$1, INT);}
    | FLOAT_CONSTANT {$$ = new Constant(*$1, FLOAT);}
;

create_query
    : CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list COMMA constraint ROUND_BRACKET_CLOSE  
    {   
        FILE *f = ((Pro *)yyget_extra(scanner))->out;
        if(txn_active){
            fprintf(f, "Warning: cannot create table in the middle of a txn\n");
            fflush(f);
            return;
        }
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        create_mutex.lock();
        checkerr(execute_create(*$3, *$5, *$7), scanner);
        changed_tables[worker_id].push_back(*$3);
        create_mutex.unlock();
    }
    | CREATE TABLE NAME ROUND_BRACKET_OPEN column_desc_list ROUND_BRACKET_CLOSE 
    {   
        FILE *f = ((Pro *)yyget_extra(scanner))->out;
        if(txn_active){
            fprintf(f, "Warning: cannot create table in the middle of a txn\n");
            fflush(f);
            return;
        }
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        create_mutex.lock();
        checkerr(execute_create(*$3, *$5), scanner);
        changed_tables[worker_id].push_back(*$3);
        create_mutex.unlock();
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
        if($2 == VARCHAR)
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
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        obtain_write_lock(worker_id, *$3);
        checkerr(execute_insert(*$3, *$6), scanner);
        changed_tables[worker_id].push_back(*$3);
        // cannot release the lock here
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
    | FLOAT_CONSTANT
    | INT_CONSTANT
;

update_query
    : UPDATE NAME SET update_list WHERE condition 
    {
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(obtain_write_lock(worker_id, *$2), scanner);
        checkerr(execute_update(*$2, *$4, $6), scanner);
        changed_tables[worker_id].push_back(*$2);
        // cannot release the lock here
    }
    | UPDATE NAME SET update_list 
    {
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(obtain_write_lock(worker_id, *$2), scanner);
        checkerr(execute_update(*$2, *$4), scanner);
        changed_tables[*(int *)yyget_extra(scanner)].push_back(*$2);
        // cannot release the lock here
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
    : DELETE FROM NAME WHERE condition 
    {
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(obtain_write_lock(worker_id, *$3), scanner);
        table_access[*$3] = worker_id;
        checkerr(execute_delete(*$3, $5), scanner);
        changed_tables[worker_id].push_back(*$3);
        // cannot release the lock here
    }
    | DELETE FROM NAME 
    {
        int worker_id = ((Pro *)yyget_extra(scanner))->id;
        checkerr(obtain_write_lock(worker_id, *$3), scanner);
        table_access[*$3] = worker_id;
        checkerr(execute_delete(*$3), scanner);
        changed_tables[worker_id].push_back(*$3);
        // cannot release the lock here
    }
;

%%

void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg){
    FILE *f = ((Pro *)yyget_extra(unused))->out;
    fprintf(f, "%s: rolling back to last commit..\n", msg); fflush(f);
    cerr<<msg<<endl;

    int worker_id = ((Pro *)yyget_extra(unused))->id;
    execute_rollback(changed_tables[worker_id]);
    txn_active = false;

    return;
}

void checkerr(int err_code, yyscan_t scanner) {
    FILE *f = ((Pro *)yyget_extra(scanner))->out;
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
            fprintf(f, "some error occured, check query. Rolling back.\n");
            cout<<"error"<<endl;
            break;
        case C_TABLE_NOT_FOUND:
            fprintf(f, "table not found\n");
            cout<<"table not found"<<endl;
            break;
        case C_FIELD_NOT_FOUND:
            fprintf(f, "field not found\n");
            cout<<"field not found"<<endl;
            break;
        case C_TABLE_ALREADY_EXISTS:
            fprintf(f, "table already exists\n");
            cout<<"table already exists"<<endl;
            break;
    }
    fflush(f);

    int worker_id = ((Pro *)yyget_extra(scanner))->id;
    if (err_code != C_OK){
        fprintf(f, "invalid query:: rolling back...\n");
        execute_rollback(changed_tables[worker_id]);
        txn_active = false;
    }
}