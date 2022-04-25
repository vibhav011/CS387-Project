#include "query.h"
#include "../dblayer/tbl.h"
#include "../dblayer/codec.h"
#include <set>
using namespace std;

extern map<string, Table*> Tbls;
extern set<int> changed_unique_ids;
extern ChangeLog change_log;

Query_Obj::Query_Obj(vector<string>* col_names, AST* cond_tree, Temp_Table* temp_table, Schema* schema) {
    this->schema = schema;
    this->col_names = col_names;
    this->temp_table = temp_table;
    this->cond_tree = cond_tree;
}

int Table_Single_Select(void *callbackObj, RecId rid, byte *row, int len) {
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
    if (change_log.find(unique_id) != change_log.end()) {
        *tr = *change_log[unique_id].new_value;
    }

    int status = cObj->cond_tree->check_condition(tr);
    if (status == C_ERROR) {
        cObj->ret_value = C_ERROR;
        return C_ERROR;
    }
    if (status == C_FALSE) return 0;

    // Expanding col_names if it is '*'
    if (cObj->col_names->at(0) == "*") {
        cObj->col_names->pop_back();
        for (int i = 0; i < cObj->schema->numColumns; i++) {
            string s(cObj->schema->columns[i]->name);
            cObj->col_names->push_back(s);
        }
    }

    Table_row *new_row = new Table_row();

    // Adding the required columns in new_row (in the order in which they are needed)
    int num_cols = cObj->col_names->size();
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < cObj->schema->numColumns; j++) {
            string s(cObj->schema->columns[i]->name);
            if (s == cObj->col_names->at(i)) {
                new_row->fields.push_back(tr->fields[j]);
            }
        }
    }
    cObj->temp_table->rows.push_back(new_row);

    return 0;
}

// For non-join selects
int execute_select1(Temp_Table *result, string table_name, vector<string>* col_names, AST* cond_tree) {
    Table* tbl = Tbls[table_name];

    Query_Obj* callbackObj = new Query_Obj(col_names, cond_tree, result, tbl->schema);
    Table_Scan(tbl, callbackObj, Table_Single_Select);

    return callbackObj->ret_value;
}

// For join selects
int execute_select2(Temp_Table *result, vector<string> table_names, vector<string>* col_names, AST* cond_tree) {
    return 0;
}

int execute_select(Temp_Table *result, vector<string> table_names, vector<string>* col_names, AST* cond_tree) {
    if (table_names.size() == 1) {
        return execute_select1(result, table_names[0], col_names, cond_tree);
    }

    return execute_select2(result, table_names, col_names, cond_tree);
}