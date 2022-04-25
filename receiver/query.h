#include "../dblayer/tbl.h"

#ifndef _QRY_H_
#define _QRY_H_
#include <vector>
#include <map>
#include <string>
using namespace std;

typedef union {
    int int_val;
    string* str_val;
    double float_val;
} Entry;

struct Table_row {
    vector<Entry> fields;
    Entry getField(int i) {
        return fields[i];
    }
};

struct Temp_Table {
    Schema* schema;
    vector<Table_row*> rows;
};

struct Log_entry {
    Table_row *old_value, *new_value;       // NULL indicates value does not exist
    enum {UPDATE, INSERT, DELETE} change_type;
};

struct Query_Obj {
    vector<string>* col_names;
    AST* cond_tree;
    Temp_Table *temp_table;
    Schema* schema;

    Query_Obj(vector<string>*, AST*, Temp_Table*, Schema*);
};

typedef map<int, Log_entry> ChangeLog;      // map from unique_id to Log_Entry
typedef map<int, int> MappingLog;           // map from unique_id to record_id

Temp_Table* execute_select(string table_name, vector<string>col_names, AST* cond_tree);

#endif