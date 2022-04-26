#include "query.h"
#include "../dblayer/tbl.h"
#include "../dblayer/codec.h"
#include <set>
using namespace std;

extern map<string, Table*> Tbls;
extern set<int> changed_unique_ids;
extern ChangeLog change_log;

Query_Obj::Query_Obj(vector<string>* col_names, AST* cond_tree, Temp_Table* temp_table, Table *tbl1, Table *tbl2) {
    this->col_names = col_names;
    this->cond_tree = cond_tree;
    this->temp_table = temp_table;
    this->tbl1 = tbl1;
    this->tbl2 = tbl2;
}

void Table_Scan(Temp_Table *tbl, void *callbackObj, ReadFunc callbackfn) {
    for (int i = 0; i < tbl->rows.size(); i++) {
        ((Query_Obj*)callbackObj)->tr1 = tbl->rows[i];
        if (callbackfn(callbackObj, 0, NULL, 0) != C_OK)
            break;
    }
}

void decode_to_table_row(Table_row *result, Schema *schema, byte *row) {
    byte *cursor = row;
    for (int i = 0; i < schema->numColumns; i++)
    {
        Entry entry;
        // decoding bytes from record for each type of column and printing them
        switch (schema->columns[i]->type)
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
        result->fields.push_back(entry);
    }
}

int Table_Single_Select(void *callbackObj, RecId rid, byte *row, int len) {
    Query_Obj* cObj = (Query_Obj *)callbackObj;
    Table_row* tr = new Table_row();

    if (cObj->tr1 != NULL) {
        *tr = *cObj->tr1;
        cObj->tr1 = NULL;
    } else {
        // Decoding the fields
        decode_to_table_row(tr, cObj->tbl1->schema, row);
        int unique_id = tr->fields[0].int_val;
        if (change_log.find(unique_id) != change_log.end()) {
            *tr = *change_log[unique_id].new_value;
        }
    }

    int status = cObj->cond_tree->check_condition(tr, cObj->tr2);

    if (status == C_ERROR) {
        cObj->ret_value = C_ERROR;
        delete tr;
        return C_ERROR;
    }
    if (status == C_FALSE) {
        delete tr;
        return 0;
    }

    // Expanding col_names if it is '*'
    if (cObj->col_names->at(0) == "*") {
        cObj->col_names->pop_back();
        for (int i = 0; i < cObj->tbl1->schema->numColumns; i++) {
            string s(cObj->tbl1->schema->columns[i]->name);
            cObj->col_names->push_back(cObj->tbl1->name + "." + s);
        }
        if (cObj->tbl2 != NULL) {
            for (int i = 0; i < cObj->tbl2->schema->numColumns; i++) {
                string s(cObj->tbl2->schema->columns[i]->name);
                cObj->col_names->push_back(cObj->tbl2->name + "." + s);
            }
        }
    }
    cObj->col_names->insert(cObj->col_names->begin(), cObj->tbl1->name+".unique_id");

    Table_row *new_row = new Table_row();
    Table_row *use_row; Schema *scm;

    // Adding the required columns in new_row (in the order in which they are needed)
    int num_cols = cObj->col_names->size();
    for (int i = 0; i < num_cols; i++) {
        string table_col = cObj->col_names->at(i);
        int pos = table_col.find(".");
        string table_name = table_col.substr(0, pos);
        string col_name = table_col.substr(pos + 1);

        scm = cObj->tbl1->schema;
        use_row = tr;
        if (cObj->tbl2 != NULL) {
           if (table_name == cObj->tbl2->name) {
                scm = cObj->tbl2->schema;
                use_row = cObj->tr2;
           }
        }
        
        if (scm == cObj->tbl1->schema && cObj->tbl1->name != table_name && table_name != "") {
            delete new_row;
            delete tr;
            cObj->ret_value = C_TABLE_NOT_FOUND;
            return C_TABLE_NOT_FOUND;
        }
        
        bool found = false;
        for (int j = 0; j < scm->numColumns; j++) {
            string s(scm->columns[i]->name);
            if (s == cObj->col_names->at(i)) {
                new_row->fields.push_back(use_row->fields[j]);
                found = true;
                break;
            }
        }
        if (!found) {
            if (table_name != "" || cObj->tbl2 == NULL) {
                delete new_row;
                delete tr;
                cObj->ret_value = C_FIELD_NOT_FOUND;
                return C_FIELD_NOT_FOUND;
            }
            scm = cObj->tbl2->schema;
            for (int j = 0; j < scm->numColumns; j++) {
                string s(scm->columns[i]->name);
                if (s == cObj->col_names->at(i)) {
                    new_row->fields.push_back(cObj->tr2->fields[j]);
                    found = true;
                    break;
                }
            }

            if (!found) {
                delete new_row;
                delete tr;
                cObj->ret_value = C_FIELD_NOT_FOUND;
                return C_FIELD_NOT_FOUND;
            }

        }
    }
    cObj->temp_table->rows.push_back(new_row);
    delete tr;

    return C_OK;
}

int Table_Single_Select_Join(void *callbackObj, RecId rid, byte *row, int len) {
    Query_Obj* cObj = (Query_Obj *)callbackObj;
    Table_row* tr = new Table_row();
    if (cObj->tr1 != NULL) {
        *tr = *cObj->tr1;
        cObj->tr1 = NULL;
    }
    else {
        // Decoding the fields
        decode_to_table_row(tr, cObj->tbl2->schema, row);
        int unique_id = tr->fields[0].int_val;
        if (change_log.find(unique_id) != change_log.end()) {
            *tr = *change_log[unique_id].new_value;
        }
    }
    cObj->tr2 = tr;
    Table_Scan(cObj->tbl1, cObj, Table_Single_Select);

    cObj->tr2 = NULL;
    delete tr;
    return cObj->ret_value;
}

int execute_select(Temp_Table *result, vector<string> table_names, vector<string>* col_names, AST* cond_tree) {
    // For non-join selects
    if (table_names.size() == 1) {
        Table* tbl = Tbls[table_names[0]];

        Query_Obj* callbackObj = new Query_Obj(col_names, cond_tree, result, tbl, NULL);
        callbackObj->tr1 = NULL;
        callbackObj->tr2 = NULL;
        callbackObj->ret_value = 0;
        Table_Scan(tbl, callbackObj, Table_Single_Select);

        return callbackObj->ret_value;
    }

    // For join selects
    Table* tbl1 = Tbls[table_names[0]];
    Table* tbl2 = Tbls[table_names[1]];

    Query_Obj* callbackObj = new Query_Obj(col_names, cond_tree, result, tbl1, tbl2);
    callbackObj->tr1 = NULL;
    callbackObj->tr2 = NULL;
    callbackObj->ret_value = 0;
    Table_Scan(tbl2, callbackObj, Table_Single_Select_Join);

    return callbackObj->ret_value;
}

void execute_update(string table_name, vector<Update_pair*>* update_list, AST* cond_tree) {
    Table* tbl = Tbls[table_name];
    vector<string>* all_cols = new vector<string>(1, "*");
    Temp_Table* result = execute_select(table_name, all_cols, cond_tree);
    for(int i=0; i<result->rows.size(); i++){
        Table_row* old_value = result->rows[i];
        Table_row* new_value = new Table_row();
        *new_value = *old_value;
        for (int j = 0; j < update_list->size(); j++)
        {
            int change_col_num = tbl->schema->getColumnNum(((*update_list)[j]->lhs).c_str());
            new_value->fields[change_col_num].str_val = new string((*update_list)[j]->rhs);
        }
        Log_entry* log_entry = new Log_entry();
        log_entry->old_value = old_value;
        log_entry->new_value = new_value;
        log_entry->change_type = UPDATE;
        int table_num = TableNum[table_name];
        ChangeLogs[table_num].insert({UIds[table_num],*log_entry});
    }
   
}