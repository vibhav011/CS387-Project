#include<string>
#include<vector>
#include<iostream>
using namespace std;

typedef union {
    int int_val;
    string* str_val;
    double float_val;
} Entry;

typedef struct {
    char *name;
    int  type;  // one of VARCHAR, INT, LONG
} ColumnDesc;

typedef struct {
    int numColumns;
    ColumnDesc **columns; // array of column descriptors
} Schema;

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

int execute_select(Temp_Table *result, vector<string> table_names, vector<string>col_names, AST* cond_tree);
AST::check_condition(Table_row *tr1, Table_row *tr2);   // tr2 = NULL in case select is not a join