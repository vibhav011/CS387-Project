#ifndef _UTILS_H_
#define _UTILS_H_

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

typedef enum {UPDATE, INSERT, DELETE} Change_type;
struct Log_entry {
    Table_row *old_value, *new_value;       // NULL indicates value does not exist
    Change_type change_type;
};


struct Table_row {
    vector<Entry> fields;
    Entry getField(int i) {
        return fields[i];
    }
};

template <typename T>
struct Range
{
    T left, right;
};

template <typename T>
struct Column_desc
{
    string name;
    int type;
    Range<T> *range;
};

struct Temp_Table {
    string name;
    Schema* schema;
    vector<Table_row*> rows;

    void set_schema_columns(vector<string> names)
    {
        for(int i=0;i<names.size();i++)
            this->schema->columns[i]->name = &(names[i])[0];
    }

    void set_name(string name)
    {
        this->name = name;
    }
};

# define table_list vector<Tabl*>
# define _AND 0
# define _OR 1

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
vector<ChangeLog> change_logs;     // objects of change logs for corresponding tables in `tables`
vector<MappingLog> mapping_logs;

#endif