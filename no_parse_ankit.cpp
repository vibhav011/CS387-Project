#include "./dblayer/tbl.h"
#include "./receiver/query.h"
#include "utils.h"
#include "ast.h"
#include <iostream>
using namespace std;

extern vector<ChangeLog> change_logs;

int main() {
    vector<string> tabel_names = {"data", "data1"};
    vector<string> col_names = {"Country", "Capital", "Population"};
    
    ColumnDesc* col1 = new ColumnDesc((char*)"Country", VARCHAR);
    ColumnDesc* col2 = new ColumnDesc((char*)"Capital", VARCHAR);
    ColumnDesc* col3 = new ColumnDesc((char*)"Population", INT);
    Temp_Table* result = new Temp_Table();

    vector<ColumnDesc*> cols = {col1, col2, col3};
    vector<string> pk = {"Country"};
    execute_create("data", cols, pk);
    cout<<"Created table: data"<<endl;

    vector<string> col_val_list1 = {"Afghanistan", "Kabul", "35530081"};
    execute_insert("data", col_val_list1);
    cout<<"Inserted first row in data"<<endl;

    vector<string> col_val_list2 = {"Albania", "Tirana", "2930187"};
    execute_insert("data", col_val_list2);
    cout<<"Inserted second row in data"<<endl;

    // execute_delete("data", NULL);

    execute_create("data1", cols, pk);
    cout<<"Created table: data1"<<endl;
    
    vector<string> col_val_list3 = {"India", "Delhi", "675567"};
    execute_insert("data1", col_val_list3);
    cout<<"Inserted first row in data1"<<endl;

    vector<string> col_val_list4 = {"China", "Beijing", "4354765"};
    execute_insert("data1", col_val_list4);
    cout<<"Inserted second row in data1"<<endl;

    vector<string> fetch_cols = {"data.Country", "data1.Capital"};
    vector<string> fetch_tables = {"data", "data1"};
    int select_exit = execute_select(result, fetch_tables, fetch_cols);
    result->prettyPrint();
    // delete result;
    // delete col_names;
    // delete table_names;
    // delete col1;
    // delete col2;
    // delete col3;
    // clean_and_exit();
    // delete col_ast;
    // delete data;
    // delete const_ast;
    // delete cond_tree;
}