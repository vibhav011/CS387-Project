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

typedef enum {UPDATE, INSERT, DELETE} Change_type;
struct Log_entry {
    Table_row *old_value, *new_value;       // NULL indicates value does not exist
    Change_type change_type;
};

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

typedef map<int, Log_entry> ChangeLog;      // map from unique_id to Log_Entry
typedef map<int, RecId> MappingLog;           // map from unique_id to record_id

vector<ChangeLog> ChangeLogs;
vector<MappingLog> MappingLogs;

int num_tables = 0;         // constantly increasing integer for keeping track of number of tables

map<string, int> TableNum;          // map from table name to table number

vector<int> UIds;           // constanstly increasing uids for each of the tables

int execute_select(Temp_Table *result, vector<string> table_names, vector<string>col_names, AST* cond_tree);

#endif