#include "query.h"
#include "../dblayer/tbl.h"
#include "../dblayer/codec.h"
#include <set>
using namespace std;

extern map<string, Table*> Tbls;
extern set<int> changed_unique_ids;

Query_Obj::Query_Obj(vector<string>* col_names, AST* cond_tree, Temp_Table* temp_table, Schema* schema) {
    this->schema = schema;
    this->col_names = col_names;
    this->temp_table = temp_table;
    this->cond_tree = cond_tree;
}

void Table_Single_Select(void *callbackObj, RecId rid, byte *row, int len) {
    Query_Obj* cObj = (Query_Obj *)callbackObj;
    Table_row* tr = new Table_row();
    // Decoding the fields
    byte *cursor = row;
    for (int i = 0; i < cObj->schema->numColumns; i++)
    {
        Entry entry;
        // decoding bytes from record for each type of column and printing them
        switch (cObj->schema->columns[i]->type)
        {
        case INT:; {
            int int_field = DecodeInt(cursor);
            entry.int_val = int_field;
            cursor += 4; // cursor offset by size of int
            break;
        }
        case VARCHAR:; {
            char string_field[256];
            int len = DecodeCString(cursor, string_field, 256); // check max len
            entry.str_val = new string(string_field);
            cursor += 2 + len; // cursor offset by 2 byte len + string length
            break;
        }
        case LONG:; {
            long long long_field = DecodeLong(cursor);
            cursor += 8; // cursor offset by size of long long
            
            break;
        }
        default:
            break;
        }
        tr->fields.push_back(entry);
    }
    int unique_id = tr->fields[0].int_val;
    if (changed_unique_ids.find(unique_id) != changed_unique_ids.end())

    if (!cObj->cond_tree->check_condition(row, len)) return;

    Schema *schema = cObj->schema;
    byte *cursor = row;
}

Temp_Table* execute_select(string table_name, vector<string>* col_names, AST* cond_tree) {
    Table* tbl = Tbls[table_name];
    Temp_Table* result = new Temp_Table();

    Query_Obj* callbackObj = new Query_Obj(col_names, cond_tree, result, tbl->schema);
    Table_Scan(tbl, callbackObj, Table_Single_Select);

    return result;
}