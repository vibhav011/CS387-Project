#include "./dblayer/tbl.h"
#include "./receiver/query.h"
#include "utils.h"
#include "ast.h"
#include "./receiver/commit.h"
#include <iostream>
using namespace std;

extern vector<ChangeLog> change_logs; 
extern vector<Table*> tables;

void clean_and_exit() {
    for (int i = 0; i < tables.size(); i++) {
        ChangeLog& log_map = change_logs[i];
        map<int, Log_Entry>:: iterator log;
        for (log = log_map.begin(); log != log_map.end(); log++) {
            if (log->second.old_value != NULL) {
                for (int j = 0; j < tables[i]->schema->numColumns; j++) {
                    if (tables[i]->schema->columns[j]->type == VARCHAR) {
                        delete log->second.old_value->getField(j).str_val;
                    }
                }
            }
            if (log->second.new_value != NULL) {
                for (int j = 0; j < tables[i]->schema->numColumns; j++) {
                    if (tables[i]->schema->columns[j]->type == VARCHAR) {
                        delete log->second.new_value->getField(j).str_val;
                    }
                }
            }
            delete log->second.old_value;
            delete log->second.new_value;
        }
        Table_Close(tables[i]);
        delete tables[i];
    }
    exit(0);
}

int main() {
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

    ColumnDesc* colsPtr[] = {col1, col2, col3};
    Schema* schema = new Schema(3, colsPtr);
    Temp_Table* result = new Temp_Table(schema);

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
    col_val_list1.push_back("3553008");
    int insert_exit = execute_insert("data", col_val_list1);
    cout<<"insert 1 done"<<endl;
    // execute_delete("data", NULL);
    execute_delete("data", NULL);
    cout << "delete done" << endl;
    vector<string> col_val_list2;
    col_val_list2.push_back("Albania");
    col_val_list2.push_back("Tirana");
    col_val_list2.push_back("2930187");
    insert_exit = execute_insert("data", col_val_list2);
    cout << "insert done" << endl;

    /*
    create_exit = execute_create("data1", cols, pk);
    cout << "create done" << endl;
    // clean_and_exit();
    // return 0;
    col_val_list1.clear();
    col_val_list1.push_back("India");
    col_val_list1.push_back("New Delhi");
    col_val_list1.push_back("32435");
    insert_exit = execute_insert("data1", col_val_list1);
    cout << "insert done" << endl;
    // execute_delete("data1", NULL);
    col_val_list2.clear();
    col_val_list2.push_back("China");
    col_val_list2.push_back("Beijing");
    col_val_list2.push_back("454657");
    insert_exit = execute_insert("data1", col_val_list2);
    cout<<"insert done"<<endl;
    */

    vector<int> *ci = new vector<int> (1, 0);
    // ci->push_back(1);
    int commit_exit = execute_commit(ci);
    cout<<"commit exited with: "<<commit_exit<<endl;

    delete result;
    delete col_names;
    delete table_names;
    delete col1;
    delete col2;
    delete col3;
    // delete ci;
    // clean_and_exit();
    // delete col_ast;
    // delete data;
    // delete const_ast;
    // delete cond_tree;
}