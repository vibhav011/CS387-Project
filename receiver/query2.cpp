#include "query.h"
#include "utils.h"
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

bool passes_range_constraints(string table_name, int column_num, string new_value) {
    if(table_name_to_id.find(table_name) == table_name_to_id.end()) return false;
    int table_num = table_name_to_id[table_name];
    Table* tbl  = tables[table_num];
    Value lb = tbl->schema->columns[column_num]->lower_bound;
    Value ub = tbl->schema->columns[column_num]->lower_bound;
    switch(tbl->schema->columns[column_num]->type) {
        case VARCHAR:
            return false;
            break;
        case INT:
            try {
                int new_int_value = atoi(new_value.c_str());
                if(lb.int_val > new_int_value || ub.int_val < new_int_value) return false;
            }
            catch(int x) {
                return false;
            }
            break;
        case LONG:
            try {
                int new_float_value = atol(new_value.c_str());
                if(lb.float_val > new_float_value || ub.float_val < new_float_value) return false;
            }
            catch(int x) {
                false;
            }
            break;
        default:
            break;
    }
    return true;
}

bool passes_pk_constraints(string table_name, Table_row* new_row) {
    if(table_name_to_id.find(table_name) == table_name_to_id.end()) return false;
    int table_num = table_name_to_id[table_name];
    Table* tbl  = tables[table_num];
    Temp_Table* result = new Temp_Table();
    int ret = execute_select(result, table_name, tbl->pk);
    for(int i=0; i<result->rows.size(); i++)
    {
        bool match = true;
        for (int j = 0; j < tbl->pk.size(); j++)
        {
            int pk_col_num = tbl->schema->getColumnNum(tbl->pk[j]);
            if(new_row->getField(pk_col_num) != result->rows[i]->getField(j)){
                match = false;
                break;
            }
        }
        if(match) {
            delete result;
            return false;
        }
    }
    delete result;
    return true;
}

int execute_update(string table_name, vector<Update_pair*>* update_list, AST* cond_tree) {
    try {
        if(table_name_to_id.find(table_name) == table_name_to_id.end()) return C_TABLE_NOT_FOUND;
        int table_num = table_name_to_id[table_name];
        Table* tbl  = tables[table_num];
        vector<string>* all_cols = new vector<string>(1, "*");
        Temp_Table* result = execute_select(table_name, all_cols, cond_tree);
        delete all_cols;
        for(int i=0; i<result->rows.size(); i++){
            Table_row* old_value = result->rows[i];
            Table_row* new_value = new Table_row();
            *new_value = *old_value;
            for (int j = 0; j < update_list->size(); j++)
            {
                int change_col_num = tbl->schema->getColumnNum(((*update_list)[j]->lhs).c_str());
                if(change_col_num == -1) {
                    return -1;
                }
                bool violates = ! passes_range_constraints(table_name, change_col_num, (*update_list)[j]->rhs);
                if(violates) {
                    return -1;
                }
                string* new_rhs = new string((*update_list)[j]->rhs);
                new_value->fields[change_col_num].str_val = new_rhs;
                violates = ! passes_pk_constraints(table_name, new_value);
                if(violates) {
                    return -1;
                }
            }
            Log_entry* log_entry = new Log_entry();
            log_entry->old_value = old_value;
            log_entry->new_value = new_value;
            log_entry->change_type = UPDATE;
            int table_num = TableNum[table_name];
            change_logs[table_num].insert({result->rows[i]->fields[0].int_val,*log_entry});
        }
        return 0;
    }
    catch(int x) {
        return -1;
    }
}

int execute_create(string table_name, vector<ColumnDesc*>* column_desc_list, vector<string*>* constraint) {
    try {
        Schema* schema = new Schema();
        schema->numColumns = column_desc_list->size();
        ColumnDesc** cols = new ColumnDesc*[schema->numColumns];
        schema->columns = cols;
       
        for (int i = 0; i < schema->numColumns; i++)
        {
            *(schema->columns[i]) = *((*column_desc_list)[i]);
        }
        Table* tbl = new Table();

        int err = Table_Open("data.db", schema, false, &tbl);
        if(err<0) {
            return -1;
        }
        for (int i = 0; i < constraint->size(); i++)
        {
            tbl->pk.push_back(*((*constraint)[i]));
        }
        table_name_to_id[table_name] = tables.size()+1;
        UIds.push_back(0);
        tables.push_back(tbl);

        // call real_execute_create as well?
        return 0;
    }
    catch (int x) {
        return -1;
    }
}

int execute_insert(string table_name, vector<string*>* column_val_list) {
    try {
        if(table_name_to_id.find(table_name) == table_name_to_id.end()) return C_TABLE_NOT_FOUND;
        int table_num = table_name_to_id[table_name];
        Table* tbl  = tables[table_num];
        Table_row* new_row = new Table_row();
        Schema* schema = tbl->schema;
        for (int i = 0; i < schema->numColumns; i++)
        {
            Entry* entry = new Entry();
            switch(schema->columns[i]->type) {
                case VARCHAR:
                    *(entry->str_val) = *((*column_val_list)[i]);
                    new_row->fields.push_back(*entry);
                    break;
                case INT:
                    entry->int_val = atoi((*((*column_val_list)[i])).c_str());
                    new_row->fields.push_back(*entry);
                    break;
                case LONG:
                    entry->float_val = atol((*((*column_val_list)[i])).c_str());
                    new_row->fields.push_back(*entry);
                    break;
                default:
                    break;
            }
        }
        bool violates = ! passes_pk_constraints(table_name, new_row);
        if(violates) {
            return -1;
        }
        for (int i = 0; i < schema->numColumns; i++)
        {

            violates = passes_range_constraints(table_name, i, *((*column_val_list)[i]));
            if(violates) {
                return -1;
            }
        }
        Log_entry* log_entry = new Log_entry();
        log_entry->old_value = NULL;
        log_entry->new_value = new_row;
        log_entry->change_type = INSERT;
        int table_num = table_name_to_id[table_name];
        UIds[table_num] += 1;
        change_logs[table_num].insert({UIds[table_num],*log_entry});
        return 0;
    }
    catch (int x) {
        return -1;
    }
}

int execute_delete(string table_name, AST* cond_tree) {
    try {
        if(table_name_to_id.find(table_name) == table_name_to_id.end()) return C_TABLE_NOT_FOUND;
        int table_num = table_name_to_id[table_name];
        Table* tbl  = tables[table_num];
        vector<string>* all_cols = new vector<string>(1, "*");
        Temp_Table* result = execute_select(table_name, all_cols, cond_tree); 
        delete all_cols;  
        for (int i = 0; i < result->rows.size(); i++)
        {
            Log_entry* log_entry = new Log_entry();
            log_entry->old_value = result->rows[i];
            log_entry->new_value = NULL;
            log_entry->change_type = DELETE;
            int table_num = table_name_to_id[table_name];
            change_logs[table_num].insert({result->rows[i]->fields[0].int_val,*log_entry});
        }
        return 0;
    }
    catch (int x) {
        return -1;
    }
}