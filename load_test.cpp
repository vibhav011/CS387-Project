#include "./dblayer/tbl.h"
#include "./receiver/query.h"
#include "./receiver/commit.h"
#include "utils.h"
#include "ast.h"
#include <iostream>
using namespace std;

extern vector<ChangeLog> change_logs; 
extern vector<Table*> tables;
extern void setup_and_recover();

void clean_and_exit() {
    for (int i = 0; i < tables.size(); i++) {
        ChangeLog& log_map = change_logs[i];
        map<int, Log_Entry>:: iterator log;
        for (log = log_map.begin(); log != log_map.end(); log++) {
            if (log->second.old_value != NULL) {
                for (int j = 0; j < tables[i]->schema->numColumns; j++) {
                    if (tables[i]->schema->columns[j]->type == VARCHAR) {
                        delete log->second.old_value->fields[j].str_val;
                    }
                }
            }
            if (log->second.new_value != NULL) {
                for (int j = 0; j < tables[i]->schema->numColumns; j++) {
                    if (tables[i]->schema->columns[j]->type == VARCHAR) {
                        delete log->second.new_value->fields[j].str_val;
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
    // setup_and_recover();
    cout<<"setup and recover done"<<endl;
    vector<string>* table_names = new vector<string> (1, "countries_load");
    // table_names->push_back("data1");
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
    Temp_Table *result2 = new Temp_Table(schema);

    vector<ColumnDesc*> cols;
    cols.push_back(col1);
    cols.push_back(col2);
    cols.push_back(col3);
    vector<string> pk;
    pk.push_back("Country");
    int create_exit = execute_create("countries_load", cols, pk);

    for(int ij = 0; ij <500; ij++){
        delete result;
        delete result2;
        vector<string> col_val_list1;
        col_val_list1.push_back("Afghanistan"+to_string(ij));
        col_val_list1.push_back("Kabul");
        col_val_list1.push_back("3553008");
        int insert_exit = execute_insert("countries_load", col_val_list1);
        cout << "insert exited with " << insert_exit<< endl;

        vector<string> col_val_list2;
        col_val_list2.push_back("Albania"+to_string(ij));
        col_val_list2.push_back("Tirana");
        col_val_list2.push_back("2930187");
        insert_exit = execute_insert("countries_load", col_val_list2);
        cout << "insert exited with " << insert_exit<< endl;

        vector<string> fetch_cols(1, "countries_load.Country");
        fetch_cols.push_back("countries_load.Population");
        fetch_cols.push_back("countries_load.Capital");
        vector<string> temp = vector<string> (1, "countries_load");
        result = new Temp_Table(schema);
        int select_exit = execute_select(result, temp, fetch_cols);
        cout<<"select exited with: "<<select_exit<<endl;
        cout<<"final result size "<< result->rows.size()<<endl;
        for (int i = 0; i < result->rows.size(); i++) {
            cout<<"row "<<i<<endl;
            for (int j = 0; j < result->schema->numColumns; j++) {
                if (result->schema->columns[j]->type == VARCHAR) {
                    // cout << "str" << endl;
                    cout<<*(result->rows[i]->fields[j].str_val)<<endl;
                } else {
                    cout<<result->rows[i]->fields[j].int_val<<endl;
                }
            }
        }

        vector<string> change_tables = vector<string>(1, "countries_load");
        int exit_commit = execute_commit(change_tables);
        cout<<"execute commit exited with "<<exit_commit<<endl;

        result2 = new Temp_Table(schema);

        select_exit = execute_select(result2, temp, fetch_cols);
        cout<<"after commit: final result size "<< result2->rows.size()<<endl;
        for (int i = 0; i < result2->rows.size(); i++) {
            cout<<"row "<<i<<endl;
            for (int j = 0; j < result2->schema->numColumns; j++) {
                if (result2->schema->columns[j]->type == VARCHAR) {
                    // cout << "str" << endl;
                    cout<<*(result2->rows[i]->fields[j].str_val)<<endl;
                } else {
                    cout<<result2->rows[i]->fields[j].int_val<<endl;
                }
            }
        }
    }

    delete result;
    delete result2;
    delete col_names;
    delete table_names;
    delete col1;
    delete col2;
    delete col3;
    // clean_and_exit();
    // delete col_ast;
    // delete data;
    // delete const_ast;
    // delete cond_tree;
}