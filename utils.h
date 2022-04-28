#ifndef _UTILS_H_
#define _UTILS_H_

#include<string>
#include<vector>
#include<map>
#include<iostream>
#include<cassert>
#include "dblayer/tbl.h"

using namespace std;

enum {C_OK, C_TRUE, C_FALSE, C_ERROR, C_TABLE_NOT_FOUND, C_FIELD_NOT_FOUND} error_codes;

typedef union {
    int int_val;
    string* str_val;
    double float_val;
} Entry;

typedef enum {_UPDATE, _INSERT, _DELETE} Change_type;

class Table_Row
{
    public:
    vector<Entry> fields;
    Entry getField(int ind);
    void addField(Entry entry);
};

class Log_Entry
{
    public:
    Table_Row *old_value, *new_value;
    Change_type change_type;

    Log_Entry();
};


/////////////////////////////
// Shared global variables //
/////////////////////////////
typedef map<int, Log_Entry> ChangeLog;
typedef map<int, int> MappingLog;

#endif