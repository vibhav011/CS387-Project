#include "./dblayer/tbl.h"
#include "./receiver/query.h"
#include "utils.h"
#include "ast.h"
#include <iostream>
using namespace std;

extern vector<ChangeLog> change_logs; 

int main() {
    Temp_Table* result = new Temp_Table();
    vector<string>* table_names = new vector<string> (1, "data");
    table_names->push_back("data1");
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

    vector<string> col_val_list1;
    col_val_list1.push_back("Afghanistan");
    col_val_list1.push_back("Kabul");
    col_val_list1.push_back("35530081");
    int insert_exit = execute_insert("data", col_val_list1);

    vector<string> col_val_list2;
    col_val_list2.push_back("Albania");
    col_val_list2.push_back("Tirana");
    col_val_list2.push_back("2930187");
    insert_exit = execute_insert("data", col_val_list2);

    create_exit = execute_create("data1", cols, pk);

    col_val_list1.clear();
    col_val_list1.push_back("India");
    col_val_list1.push_back("New Delhi");
    col_val_list1.push_back("32435");
    insert_exit = execute_insert("data1", col_val_list1);

    col_val_list2.clear();
    col_val_list2.push_back("China");
    col_val_list2.push_back("Beijing");
    col_val_list2.push_back("454657");
    insert_exit = execute_insert("data1", col_val_list2);

    // Log_entry le = change_logs[1][1];
    // cout<<le.new_value->fields[0].int_val<<endl;
    // cout<<*(le.new_value->fields[1].str_val)<<endl;
    // cout<<*(le.new_value->fields[2].str_val)<<endl;
    // cout<<le.new_value->fields[3].int_val<<endl;

    // ColAST* col_ast = new ColAST("data.Country");
    // Constant* data = new Constant("Albania", _TEXT);
    // cout<<"here?"<<endl;
    // ConstAST* const_ast = new ConstAST(data);
    // RelAST* cond_tree = new RelAST(col_ast, const_ast, _EQ);
    // cout<<"calling seletc"<<endl;

    vector<string> fetch_cols = {"data.Country", "data1.Capital"};

    int select_exit = execute_select(result, {"data", "data1"}, fetch_cols);
    cout<<"final result size "<< result->rows.size()<<endl;
    cout<<"select exited with: "<<select_exit<<endl;
    delete result;
    delete col_names;
    delete table_names;
    delete col1;
    delete col2;
    delete col3;
    // delete col_ast;
    // delete data;
    // delete const_ast;
    // delete cond_tree;
}