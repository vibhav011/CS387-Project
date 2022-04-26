#include "../dblayer/tbl.h"

#ifndef _QRY_H_
#define _QRY_H_
#include <vector>
#include <map>
#include <string>
using namespace std;

struct Query_Obj {
    vector<string> *col_names;
    AST *cond_tree;
    Temp_Table *temp_table;
    Table *tbl1, *tbl2;
    Table_row *tr1, *tr2;
    int ret_value;

    Query_Obj(vector<string>*, AST*, Temp_Table*, Table*, Table*);
};

struct Update_pair {
    string lhs;
    string rhs;
};

struct Temp_Table {
    string name;
    Schema* schema;
    vector<Table_row*> rows;

    void set_schema_columns(vector<string> names)
    {
        for(int i=0;i<names.size();i++)
            this->schema->columns[i]->name = &(names[i])[0];
    }

    void set_name(string name)
    {
        this->name = name;
    }
};

# define table_list vector<Temp_Table*>


int execute_select(Temp_Table *result, vector<string> table_names, vector<string> col_names, AST* cond_tree);

int execute_create_temp(table_list tables);

#endif