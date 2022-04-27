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

    vector<string> col_val_list1;
    col_val_list1.push_back("Afghanistan");
    col_val_list1.push_back("Kabul");
    col_val_list1.push_back("35530081");
    int insert_exit = execute_insert("data", col_val_list1);
    cout<<"insert exited with ret val: "<<insert_exit<<endl;
    vector<string> col_val_list2;
    col_val_list2.push_back("Albania");
    col_val_list2.push_back("Tirana");
    col_val_list2.push_back("2930187");
    insert_exit = execute_insert("data", col_val_list2);
    cout<<"insert exited with ret val: "<<insert_exit<<endl;

    Log_entry le = change_logs[0][0];
    cout<<"here"<<endl;
    cout<<le.new_value->fields.size()<<endl;
    for (int i = 0; i < le.new_value->fields.size(); i++)
    {
        switch(result->schema->columns[i]->type) {
            case VARCHAR:
                cout<<"here?"<<endl;
                cout<<*(le.new_value->fields[i].str_val)<<endl;
                break;
            case INT:
                cout<<le.new_value->fields[i].int_val<<endl;
                break;
            case DOUBLE:
                cout<<le.new_value->fields[i].float_val<<endl;
                break;
        }
        
    }
    
    

    ColAST* col_ast = new ColAST("data.Country");
    Constant* data = new Constant("Albania", _TEXT);
    cout<<"here?"<<endl;
    ConstAST* const_ast = new ConstAST(data);
    RelAST* cond_tree = new RelAST(col_ast, const_ast, _EQ);
    cout<<"calling seletc"<<endl;
    int select_exit = execute_select(result, *table_names, *col_names, cond_tree);
    cout<<"final result size "<< result->rows.size()<<endl;
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