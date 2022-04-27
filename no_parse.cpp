#include "./dblayer/tbl.h"
#include "./receiver/query.h"
#include "utils.h"
#include "ast.h"
#include <iostream>
using namespace std;

int main() {
    Temp_Table* result = new Temp_Table();
    vector<string>* table_names = new vector<string> (1, "data");
    vector<string>* col_names = new vector<string> (1, "Country");
    col_names->push_back("Capital");
    col_names->push_back("Population");
    char nm1[] = "Country";
    char nm2[] = "Capital";
    char nm3[] = "Population";
    ColumnDesc* col1 = new ColumnDesc(nm1, VARCHAR);
    ColumnDesc* col2 = new ColumnDesc(nm2, VARCHAR);
    ColumnDesc* col3 = new ColumnDesc(nm3, INT);
    vector<ColumnDesc*> cols;
    cols.push_back(col1);
    cols.push_back(col2);
    cols.push_back(col3);
    vector<string> pk;
    pk.push_back("Country");
    int create_exit = execute_create("data", cols, pk);
    cout<<"create exited with ret val: "<<create_exit<<endl;
    ColAST* col_ast = new ColAST("Country");
    Constant* data = new Constant("Albania", _TEXT);
    ConstAST* const_ast = new ConstAST(data);
    RelAST* cond_tree = new RelAST(col_ast, const_ast, _EQ);
    int select_exit = execute_select(result, *table_names, *col_names, cond_tree);
    cout<<"select exited with: "<<select_exit<<endl;
    delete result;
    delete col_names;
    delete table_names;
    delete col1;
    delete col2;
    delete col3;
    delete col_ast;
    delete data;
    delete const_ast;
    delete cond_tree;
}