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


int execute_select(Temp_Table *result, vector<string> table_names, vector<string>col_names, AST* cond_tree);

#endif