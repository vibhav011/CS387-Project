#include "./dblayer/tbl.h"
#include "./receiver/query.h"
#include "./receiver/commit.h"
#include "utils.h"
#include "ast.h"
#include <iostream>
using namespace std;
extern vector<Table*> tables;

extern vector<ChangeLog> change_logs;

int main() {
    vector<string> table_names = {"data", "data1"};
    vector<string> col_names = {"Country", "Capital", "Pop"};
    
    ColumnDesc* col1 = new ColumnDesc((char*)"Country", VARCHAR);
    ColumnDesc* col2 = new ColumnDesc((char*)"Capital", VARCHAR);
    ColumnDesc* col3 = new ColumnDesc((char*)"Pop", INT);
    Temp_Table* result = new Temp_Table();

    vector<ColumnDesc*> cols = {col1, col2, col3};
    vector<string> pk = {"Country"};
    execute_create("data", cols, pk);
    cout<<"Created table: data"<<endl;

    vector<string> col_val_list1 = {"Afghanistan", "Kabul", "35530081"};
    execute_insert("data", col_val_list1);
    cout<<"Inserted first row in data"<<endl;

    // execute_delete("data", NULL);

    vector<string> col_val_list2 = {"Albania", "Tirana", "2930187"};
    execute_insert("data", col_val_list2);
    cout<<"Inserted second row in data"<<endl;

    execute_create("data1", cols, pk);
    cout<<"Created table: data1"<<endl;
    
    vector<string> col_val_list3 = {"India", "Delhi", "675567"};
    execute_insert("data1", col_val_list3);
    cout<<"Inserted first row in data1"<<endl;

    vector<string> col_val_list4 = {"China", "Beijing", "4354765"};
    execute_insert("data1", col_val_list4);
    cout<<"Inserted second row in data1"<<endl;

    ColAST* col_ast = new ColAST("data.Pop");
    Constant* data = new Constant("34", INT);
    ConstAST* const_ast = new ConstAST(data);
    RelAST* cond_tree = new RelAST(col_ast, const_ast, _GT);

    vector<string> fetch_cols = {"Country"};
    vector<string> fetch_tables = {"data"};
    execute_select(result, fetch_tables, fetch_cols);
    result->prettyPrint();

    vector<int> v = {0};
    execute_commit(&v);
    cout<<"Executed commit on data"<<endl;

    execute_select(result, fetch_tables, fetch_cols, cond_tree);
    result->prettyPrint();
}