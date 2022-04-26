#include<string>
#include<vector>
#include<map>
#include<iostream>
#include "dblayer/tbl.h"

using namespace std;

typedef union {
    int int_val;
    string* str_val;
    double float_val;
} Entry;

// typedef struct {
//     char *name;
//     int  type;  // one of VARCHAR, INT, LONG
// } ColumnDesc;

// typedef struct {
//     int numColumns;
//     ColumnDesc **columns; // array of column descriptors
// } Schema;

struct Log_entry {
    Table_row *old_value, *new_value;       // NULL indicates value does not exist
    enum {UPDATE, INSERT, DELETE} change_type;
};


struct Table_Row {
    vector<Entry> fields;
    Entry getField(int i) {
        return fields[i];
    }
};

int execute_select(Temp_Table *result, vector<string> table_names, vector<string>col_names, AST* cond_tree);
AST::check_condition(Table_row *tr1, Table_row *tr2);   // tr2 = NULL in case select is not a join

enum {C_OK, C_TRUE, C_FALSE, C_ERROR, C_TABLE_NOT_FOUND, C_FIELD_NOT_FOUND} error_codes;

/////////////////////////////
// Shared global variables //
/////////////////////////////
typedef map<int, Log_entry> ChangeLog;
typedef map<int, int> MappingLog;

map<string, int> table_name_to_id;
vector<Table*> tables;              // objects of all tables
vector<int> UIds;                   // constanstly increasing uids for each of the tables
vector<ChangeLog*> change_logs;     // objects of change logs for corresponding tables in `tables`
vector<MappingLog*> mapping_logs;