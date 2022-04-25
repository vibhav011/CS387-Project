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
    string name;
    Schema* schema;
    vector<Table_row*> rows;

    void set_schema_columns(vector<string> names)
    {
        for(int i=0;i<names.size();i++)
            schema->columns[i]->name = &(names[i])[0];
    }

    void set_name(string name)
    {
        name = name;
    }
};

# define table_list vector<Tabl*>

int execute_select(Temp_Table *result, vector<string> table_names, vector<string>col_names, AST* cond_tree);
AST::check_condition(Table_row *tr1, Table_row *tr2);   // tr2 = NULL in case select is not a join

/////////////////////////////
// Shared global variables //
/////////////////////////////

map<string, int> table_name_to_id;
vector<Table*> tables;              // objects of all tables
vector<ChangeLog*> change_logs;     // objects of change logs for corresponding tables in `tables`
vector<MappingLog*> mapping_logs;