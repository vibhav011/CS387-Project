#include "query.h"
#include "utils.h"
#include "../dblayer/tbl.h"
#include "../dblayer/codec.h"
#include <set>
using namespace std;

extern map<string, Table*> Tbls;
extern set<int> changed_unique_ids;

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