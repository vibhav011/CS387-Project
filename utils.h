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

struct Table_Row {
    vector<Entry> fields;
    Entry getField(int i) {
        return fields[i];
    }
};

typedef enum {_UPDATE, _INSERT, _DELETE} Change_type;
struct Log_entry {
    Table_Row *old_value, *new_value;       // NULL indicates value does not exist
    Change_type change_type;
};

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