#include <set>
#include <iostream>
#include <unistd.h>
#include "query.h"
#include "utils.h"
#include "../dblayer/codec.h"
using namespace std;

extern map<string, int> table_name_to_id;
extern vector<Table*> tables;              // objects of all tables
extern vector<int> UIds;                   // constanstly increasing uids for each of the tables
extern vector<ChangeLog> change_logs;     // objects of change logs for corresponding tables in `tables`
extern vector<MappingLog> mapping_logs;
extern map<string, int> table_access;
extern list<mutex> locks;

int execute_create_temp(table_list tables){return C_OK;}

Query_Obj::Query_Obj(vector<string> col_names, CondAST* cond_tree, Temp_Table* temp_table, int tbl1_id, int tbl2_id) {
    this->col_names = col_names;
    this->cond_tree = cond_tree;
    this->temp_table = temp_table;
    this->tbl1_id = tbl1_id;
    this->tbl2_id = tbl2_id;
}

void delete_table_row_fields(Table_Row *tr, Schema *schema) {
    for (int i = 0; i < schema->numColumns; i++) {
        if (schema->columns[i]->type == VARCHAR)
            delete tr->getField(i).str_val;
    }
}

void decode_to_table_row(Table_Row *result, Schema *schema, Byte *row) {
    Byte *cursor = row;
    for (int i = 0; i < schema->numColumns; i++)
    {
        Entry entry;
        // decoding bytes from record for each type of column and printing them
        switch (schema->columns[i]->type)
        {
        case INT: {
            int int_field = DecodeInt(cursor);
            entry.int_val = int_field;
            cursor += 4; // cursor offset by size of int
            break;
        }
        case VARCHAR: {
            char string_field[256];
            int len = DecodeCString(cursor, string_field, 256); // check max len
            entry.str_val = new string(string_field);
            cursor += 2 + len; // cursor offset by 2 Byte len + string length
            break;
        }
        case DOUBLE: {
            double long_field = DecodeDouble(cursor);
            entry.float_val = long_field;
            cursor += 8; // cursor offset by size of long long
            
            break;
        }
        
        default:
            break;
        }
        result->addField(entry);
    }
}

int Table_Single_Select(void *callbackObj, RecId rid, Byte *row, int len) {
    Query_Obj* cObj = (Query_Obj *)callbackObj;
    Table_Row* tr = new Table_Row();
    Table* tbl1 = tables[cObj->tbl1_id];
    Table *tbl2 = NULL;
    if(cObj->tbl2_id != -1)
        tbl2 = tables[cObj->tbl2_id];

    if(cObj->tr1 != NULL) {
        *tr = *cObj->tr1;
        cObj->tr1 = NULL;
    } else {
        // Decoding the fields
        decode_to_table_row(tr, tbl1->schema, row);
        int unique_id = tr->getField(0).int_val;
        ChangeLog& change_log = change_logs[cObj->tbl1_id];
        if (change_log.find(unique_id) != change_log.end() && (cObj->user_id == -1 || cObj->user_id == table_access[tbl1->name])) {
            delete_table_row_fields(tr, tbl1->schema);
            if (change_log[unique_id].change_type == _DELETE) { // The row is deleted
                delete tr;
                return C_OK;
            }
            *tr = *change_log[unique_id].new_value;
        }
    }
    int ret = query_process(cObj, tr, rid);
   
    delete tr;
    return ret;
}

int Table_Single_Select_Join(void *callbackObj, RecId rid, Byte *row, int len) {
    Query_Obj* cObj = (Query_Obj *)callbackObj;
    Table_Row* tr = new Table_Row();

    if (cObj->tr1 != NULL) {            // The second table is temp_table
        *tr = *cObj->tr1;
        cObj->tr1 = NULL;
    } else {
        // Decoding the fields
        Table* tbl2 = tables[cObj->tbl2_id];
        decode_to_table_row(tr, tbl2->schema, row);
        
        int unique_id = tr->getField(0).int_val;
        ChangeLog& change_log = change_logs[cObj->tbl2_id];
        if(change_log.find(unique_id) != change_log.end() && (cObj->user_id == -1 || cObj->user_id == table_access[tbl2->name])) {
            delete_table_row_fields(tr, tbl2->schema);
            if (change_log[unique_id].change_type == _DELETE) {    // The row is deleted
                delete tr;
                return C_OK;
            }
            *tr = *change_log[unique_id].new_value;
        }
    }
    
    cObj->tr2 = tr;
    Table* tbl1 = tables[cObj->tbl1_id];
    Table_Scan(tbl1, cObj, Table_Single_Select);
    if (cObj->user_id == -1 || table_access[tbl1->name] == cObj->user_id)
        log_scan(cObj);
    
    cObj->tr2 = NULL;
    delete tr;
    return cObj->ret_value;
}

int log_scan(Query_Obj *cObj)
{
    ChangeLog &logs = change_logs[cObj->tbl1_id];
    map<int, Log_Entry>:: iterator entry = logs.begin();
    for(entry = logs.begin(); entry != logs.end(); entry++)
    {
        if(entry->second.change_type != _INSERT)
            continue;
        Table_Row *tr = new Table_Row();
        *tr = *(entry->second.new_value);
        int err = query_process(cObj, tr, -1);
        delete tr;
        if(err != 0)
            return err;
    }
    return C_OK;
}

int log_scan_join(Query_Obj *cObj)
{
    ChangeLog &logs = change_logs[cObj->tbl2_id];
    map<int, Log_Entry>:: iterator entry = logs.begin();
    for(entry = logs.begin(); entry != logs.end(); entry++)

    {
        if(entry->second.change_type != _INSERT)
            continue;
        Table_Row *tr = new Table_Row();
        *tr = *(entry->second.new_value);
        cObj->tr2 = tr;
        Table *tbl1 = tables[cObj->tbl1_id];
        Table_Scan(tbl1, cObj, Table_Single_Select);
        if (cObj->user_id == -1 || table_access[tbl1->name] == cObj->user_id)
            log_scan(cObj);
        cObj->tr2 = NULL;
        delete tr;
    }
    return C_OK;
}

int query_process(Query_Obj *cObj, Table_Row *tr, RecId rid)
{
    Table *tbl1 = tables[cObj->tbl1_id];
    Table *tbl2 = NULL;

    if (cObj->tbl2_id != -1)
        tbl2 = tables[cObj->tbl2_id];

    if(cObj->cond_tree != NULL)
    {
        int status;
        if(tbl2 == NULL)
            status = cObj->cond_tree->check_row(tr, tbl1->schema, cObj->tr2);
        else
            status = cObj->cond_tree->check_row(tr, tbl1->schema, cObj->tr2, tbl2->schema);
        if (status == C_ERROR) {
            cObj->ret_value = C_ERROR;
            return C_ERROR;
        }

        if (status == false) {
            return C_OK;
        }
    }
    
    Table_Row *new_row = new Table_Row();
    Table_Row *use_row; 
    Schema *scm;

    // Adding the required columns in new_row (in the order in which they are needed)
    int num_cols = cObj->col_names.size();

    for(int i=0;i<num_cols;i++) {
        string table_col = cObj->col_names[i];
        int pos = table_col.find(".");
        string table_name, col_name;
        if(pos == string::npos)
            col_name = table_col;
        else
        {
            table_name = table_col.substr(0, pos);
            col_name = table_col.substr(pos+1);
        }

        scm = tbl1->schema;
        use_row = tr;
        if(tbl2 != NULL) {
            if(table_name == tbl2->name) {
                scm = tbl2->schema;
                use_row = cObj->tr2;
           }
        }

        if(scm == tbl1->schema && tbl1->name != table_name && table_name != "") {
            delete new_row;
            cObj->ret_value = C_TABLE_NOT_FOUND;
            return C_TABLE_NOT_FOUND;
        }
        
        bool found = false;
        for(int j=0;j<scm->numColumns;j++) {
            string s = scm->columns[j]->name;
            if(s == col_name) {
                new_row->addField(use_row->getField(j));
                found = true;
                break;
            }
        }
        
        if (!found) {
            if(table_name != "" || tbl2 == NULL) {
                delete new_row;
                cObj->ret_value = C_FIELD_NOT_FOUND;
                return C_FIELD_NOT_FOUND;
            }

            scm = tbl2->schema;
            for(int j=0;j<scm->numColumns;j++) {
                string s(scm->columns[i]->name);
                if(s == cObj->col_names[i]) {
                    new_row->addField(cObj->tr2->getField(j));
                    found = true;
                    break;
                }
            }

            if (!found) {
                delete new_row;
                cObj->ret_value = C_FIELD_NOT_FOUND;
                return C_FIELD_NOT_FOUND;
            }
        }
    }
    if(new_row->fields[0].int_val != -1) cObj->temp_table->rows.push_back(new_row);
    // cout<<new_row->fields[0].int_val<<endl;
    // if(new_row->fields[0].int_val == -1) cout<<"yahan theek kardo"<<endl;
    // cObj->temp_table->rows.push_back(new_row);
    if (cObj->rids != NULL)
        cObj->rids->push_back(rid);
    return C_OK;
}

int execute_select(Temp_Table *result, vector<string> table_names, vector<string> col_names, CondAST *cond_tree, vector<int> *rids, int user_id) {
    for (int i = 0; i < table_names.size(); i++)
    {
        if(table_name_to_id.find(table_names[i]) == table_name_to_id.end())
            return C_TABLE_NOT_FOUND;
    }

    int tbl1_id = table_name_to_id[table_names[0]], tbl2_id = -1;
    if(table_names.size() > 1)
        tbl2_id = table_name_to_id[table_names[1]];


    if(find(col_names.begin(), col_names.end(), "*") != col_names.end())
    {
        col_names.clear();
        Table *tbl = tables[tbl1_id];
        for(int i=1;i<tbl->schema->numColumns;i++)
            col_names.push_back(table_names[0]+"."+tbl->schema->columns[i]->name);
        if(tbl2_id != -1)
        {
            tbl = tables[tbl2_id];
            for(int i=1;i<tbl->schema->numColumns;i++)
                col_names.push_back(table_names[1]+"."+tbl->schema->columns[i]->name);
        }
    }
    
    if(tbl2_id != -1)
        col_names.insert(col_names.begin(), table_names[1]+".unique_id");
    col_names.insert(col_names.begin(), table_names[0]+".unique_id");

    vector<pair<string, int> > types;

    // Populate the types vector
    for(int i=0;i<col_names.size();i++) {
        string table_col = col_names[i];
        int pos = table_col.find(".");
        string table_name, col_name;
        if(pos == string::npos)
            col_name = table_col;
        else
        {
            table_name = table_col.substr(0, pos);
            col_name = table_col.substr(pos+1);
        }

        Table *tbl = tables[tbl1_id];
        int col_num = -1;
        if(table_name != "")
        {
            int ind = find(table_names.begin(), table_names.end(), table_name) - table_names.begin();
            if(ind == table_names.size())
                return C_TABLE_NOT_FOUND;
            else if(ind == 0)
                tbl = tables[tbl1_id];
            else
                tbl = tables[tbl2_id];
            col_num = tbl->schema->getColumnNum(col_name.c_str());
        }
        else
        {
            col_num = tbl->schema->getColumnNum(col_name.c_str());
            if(col_num == -1 && tbl2_id != -1)
            {
                tbl = tables[tbl2_id];
                col_num = tbl->schema->getColumnNum(col_name.c_str());
            }
        }
        
        if(col_num == -1)
            return C_FIELD_NOT_FOUND;
    
        types.push_back(make_pair(tbl->name+"."+col_name, tbl->schema->columns[col_num]->type));
    }

    ColumnDesc** cols = new ColumnDesc*[types.size()];
    Schema* schema = new Schema(types.size(), cols);
    
    for(int i=0;i<schema->numColumns;i++)
        schema->columns[i] = new ColumnDesc((char*)types[i].first.c_str(), types[i].second);

    // For non-join selects
    if (table_names.size() == 1) {
        Table* tbl = tables[tbl1_id];

        Query_Obj* callbackObj = new Query_Obj(col_names, cond_tree, result, tbl1_id, -1);
        callbackObj->tr1 = NULL;
        callbackObj->tr2 = NULL;
        callbackObj->ret_value = 0;
        callbackObj->temp_table->schema = schema;
        callbackObj->rids = rids;
        callbackObj->user_id = user_id;
        Table_Scan(tbl, callbackObj, Table_Single_Select);
        if(user_id == -1 || table_access[table_names[0]] == user_id)
            log_scan(callbackObj);
        int retval = callbackObj->ret_value;
        delete callbackObj;
        return retval;
    }

    // For join selects
    if (table_name_to_id.find(table_names[0]) == table_name_to_id.end() ||
        table_name_to_id.find(table_names[1]) == table_name_to_id.end()) {
        return C_TABLE_NOT_FOUND;
    }

    Table* tbl1 = tables[tbl1_id];
    Table* tbl2 = tables[tbl2_id];

    Query_Obj* callbackObj = new Query_Obj(col_names, cond_tree, result, tbl1_id, tbl2_id);
    callbackObj->tr1 = NULL;
    callbackObj->tr2 = NULL;
    callbackObj->ret_value = 0;
    callbackObj->temp_table->schema = schema;
    callbackObj->rids = NULL;
    callbackObj->user_id = user_id;
    Table_Scan(tbl2, callbackObj, Table_Single_Select_Join);
    if (user_id == -1 || table_access[table_names[1]] == user_id)
        log_scan_join(callbackObj);
    delete callbackObj; // TODO: Delete callbackObj properly
    return callbackObj->ret_value;
}

bool passes_range_constraints(string table_name, int column_num, string value) {

    if(table_name_to_id.find(table_name) == table_name_to_id.end()) 
        return true;

    int table_id = table_name_to_id[table_name];
    Table* tbl  = tables[table_id];
    if(!tbl->schema->columns[column_num]->range)
    {
        return true;
    }

    Value lb = tbl->schema->columns[column_num]->lower_bound;
    Value ub = tbl->schema->columns[column_num]->upper_bound;

    switch(tbl->schema->columns[column_num]->type) {
        case INT:
            try {
                int new_int_value = stoi(value);
                if(lb.int_val > new_int_value || ub.int_val < new_int_value) 
                    return false;
            }
            catch(int x) {
                return false;
            }
            break;
        case DOUBLE:
            try {
                int new_float_value = stof(value);
                if(lb.float_val > new_float_value || ub.float_val < new_float_value) 
                    return false;
            }
            catch(int x) {
                return false;
            }
            break;
        default:
            break;
    }
    return true;
}

bool passes_pk_constraints(string table_name, Table_Row* row) {
    if(table_name_to_id.find(table_name) == table_name_to_id.end()) 
        return false;

    int table_id = table_name_to_id[table_name];
    Table* tbl  = tables[table_id];
    if(tbl->pk.size() == 0)
        return true;

    Temp_Table* result = new Temp_Table(tbl->schema);
    int ret = execute_select(result, vector<string> (1, table_name), tbl->pk, NULL);

    if(ret != C_OK)
        return false;

    bool found = false;
    for(int i=0;i<result->rows.size();i++)
    {
        bool match = true;
        if(result->rows[i]->fields[0].int_val == row->fields[0].int_val){
            // cout<<"found it"<<endl;
            continue;
        }
        for(int j=0;j<tbl->pk.size();j++)
        {
            int pk_col_num = tbl->schema->getColumnNum(tbl->pk[j].c_str());
            switch(tbl->schema->columns[pk_col_num]->type) {
                case VARCHAR:
                    // cout<<"in match"<<endl;
                    // cout<<"new row : "<<*(row->getField(pk_col_num).str_val)<<endl;
                    // cout<<"old row: "<<*(result->rows[i]->getField(pk_col_num).str_val)<<endl;
                    if(*(row->getField(pk_col_num).str_val) != *(result->rows[i]->getField(pk_col_num).str_val))
                        match = false;
                    break;
                case INT:
                    if(row->getField(pk_col_num).int_val != result->rows[i]->getField(pk_col_num).int_val)
                        match = false;
                    break;
                case DOUBLE:
                    if(row->getField(pk_col_num).float_val != result->rows[i]->getField(pk_col_num).float_val)
                        match = false;
                    break;
                default:
                    break;
            }
            if(!match) 
                break;
        }
        if(match) 
        {
            found = true;
            break;
        }
    }
    delete result;
    return !found;
}

int execute_update(string table_name, vector<Update_Pair*> &update_list, CondAST* cond_tree) {
    try {
        ChangeLog temp_cl;
        // cout<<"all in uopdate"<<endl<<endl;
        if(table_name_to_id.find(table_name) == table_name_to_id.end()) 
            return C_TABLE_NOT_FOUND;

        int table_id = table_name_to_id[table_name];
        Table* tbl  = tables[table_id];
        
        Temp_Table* result = new Temp_Table(tbl->schema);
        int ret = execute_select(result, vector<string> (1, table_name), vector<string> (1, "*"), cond_tree);
        if (ret != C_OK) {
            delete result;
            return ret;
        }

        for(int i=0; i<result->rows.size(); i++){
            Table_Row* old_value = result->rows[i];
            Table_Row* new_value = new Table_Row();
            *new_value = *old_value;
            for (int j = 0; j < update_list.size(); j++)
            {
                // cout<<update_list[j]->lhs<<endl;
                int change_col_num = tbl->schema->getColumnNum(update_list[j]->lhs.c_str());
                if(change_col_num == -1) {
                    delete new_value;
                    return -1;
                }

                if(!passes_range_constraints(table_name, change_col_num, update_list[j]->rhs)) {
                    delete result;
                    for (int jj = 0; jj < j; jj++) {
                        int change_col_num = tbl->schema->getColumnNum(update_list[jj]->lhs.c_str());
                        if (tbl->schema->columns[change_col_num]->type == VARCHAR) {
                            delete new_value->fields[change_col_num].str_val;
                        }
                    }
                    cout<<"no for rc"<<endl;
                    delete new_value;
                    return -1;
                }

                string new_rhs(update_list[j]->rhs);
                switch (tbl->schema->columns[change_col_num]->type)
                {
                    case INT:
                        try {
                            int new_int_value = atoi((new_rhs).c_str());
                            new_value->fields[change_col_num].int_val = new_int_value;
                        }
                        catch(int x) {
                            return -1;
                        }
                        break;
                    case DOUBLE:
                        try {
                            double new_float_value = atof((new_rhs).c_str());
                            new_value->fields[change_col_num].float_val = new_float_value;
                        }
                        catch(int x) {
                            return -1;
                        }
                        break;
                    case VARCHAR:
                        new_value->fields[change_col_num].str_val = new string(new_rhs);
                        break;
                    default:
                        break;
                }
            }

            if(!passes_pk_constraints(table_name, new_value)) {
                for (int jj = 0; jj < i; jj++)
                {
                    if(temp_cl.find(result->rows[jj]->fields[0].int_val) != temp_cl.end()) {
                        change_logs[table_id][result->rows[jj]->fields[0].int_val].new_value = temp_cl[result->rows[jj]->fields[0].int_val].old_value;
                    }
                    else{
                        continue;
                    }
                }
                delete result;
                for (int jj = 0; jj < update_list.size(); jj++) {
                    int change_col_num = tbl->schema->getColumnNum(update_list[jj]->lhs.c_str());
                    if (tbl->schema->columns[change_col_num]->type == VARCHAR) {
                        delete new_value->fields[change_col_num].str_val;
                    }
                }

                delete new_value;
                return -1;
            }

            ChangeLog &change_log = change_logs[table_id];
            int unqiue_id = result->rows[i]->fields[0].int_val;

            if (change_log.find(unqiue_id) != change_log.end()) {
                Log_Entry* tle = new Log_Entry();
                tle->old_value = change_log[unqiue_id].new_value;
                change_log[unqiue_id].new_value = new_value;
                // cout<<"in cl: "<<*(change_log[unqiue_id].new_value->fields[1].str_val)<<endl;
                temp_cl[unqiue_id] = *tle;
                delete tle;
            }
            else {
                Log_Entry* log_entry = new Log_Entry();
                log_entry->old_value = new Table_Row();
                *(log_entry->old_value) = *old_value;
                log_entry->new_value = new_value;
                log_entry->change_type = _UPDATE;
                change_log[unqiue_id] = *log_entry; 
                // cout<<"in cl: "<<*(change_log[unqiue_id].new_value->fields[1].str_val)<<endl;
                delete log_entry;
            }
        }
        delete result;
        return C_OK;
    }
    catch(int x) {
        return -1;
    }
    return 0;
}

int execute_create(string table_name, vector<ColumnDesc*> &column_desc_list, vector<string> constraint) {
    try {
        if(table_name_to_id.find(table_name) != table_name_to_id.end())
            return C_TABLE_ALREADY_EXISTS;

        string schemaStr = "unique_id:int";
        string col_type;

        ColumnDesc** cols = new ColumnDesc*[column_desc_list.size()+1];
        Schema* schema = new Schema(column_desc_list.size()+1, cols, table_name);

        char *unq = (char*) malloc(sizeof(char)*10);
        sprintf(unq, "unique_id");

        schema->columns[0] = new ColumnDesc(unq, INT);
        for(int i=1;i<schema->numColumns;i++) {
            if(column_desc_list[i-1]->range) schema->columns[i] = new ColumnDesc(column_desc_list[i-1]->name, column_desc_list[i-1]->type, column_desc_list[i-1]->lower_bound, column_desc_list[i-1]->upper_bound);
            else schema->columns[i] = new ColumnDesc(column_desc_list[i-1]->name, column_desc_list[i-1]->type);
            // *schema->columns[i] = *column_desc_list[i];
            switch (column_desc_list[i-1]->type)
            {
                case INT:
                    col_type = "int";
                    break;
                case DOUBLE:
                    col_type = "double";
                    break;
                case VARCHAR:
                    col_type = "varchar";
                    break;
                default:
                    break;
            }
            schemaStr += "," + string(column_desc_list[i-1]->name) + ":" + col_type;
        }

        for (string s: constraint) {
            if (schema->getColumnNum(s.c_str()) == -1) {
                // delete schema;
                return C_FIELD_NOT_FOUND;
            }
        }

        Table* tbl = new Table();
        int err = Table_Open(&(DATA_PATH+table_name+".tbl")[0], schema, false, &tbl);
        if(err<0) {
            delete tbl;
            return -1;
        }
        FILE *fp = fopen((DATA_PATH+table_name+".scm").c_str(), "w");
        fprintf(fp, "%s\n", schemaStr.c_str());
        string constraint_str = "";
        for(int i=0;i<constraint.size();i++) {
            constraint_str += constraint[i] + " ";
        }
        fprintf(fp, "%ld ", constraint.size());
        fprintf(fp, "%s\n", constraint_str.c_str());
        string range_str = "";
        for (int i = 1; i < schema->numColumns; i++)
        {
            range_str+=to_string(column_desc_list[i-1]->range);
            if(column_desc_list[i-1]->range){
                range_str+=" ";
                switch(column_desc_list[i-1]->type) {
                    case INT:
                        range_str+=to_string(column_desc_list[i-1]->lower_bound.int_val);
                        range_str += " ";
                        range_str+=to_string(column_desc_list[i-1]->upper_bound.int_val);
                        break;
                    case DOUBLE:
                        range_str+=to_string(column_desc_list[i-1]->lower_bound.float_val);
                        range_str+=" ";
                        range_str+=to_string(column_desc_list[i-1]->upper_bound.float_val);
                        break;
                }
            }
            range_str+="\n";
        }
        fprintf(fp, "%s", range_str.c_str());
        fclose(fp);

        tbl->name = table_name;
        tbl->pk = constraint;
        table_name_to_id[table_name] = tables.size();
        UIds.push_back(0);
        tables.push_back(tbl);
        ChangeLog chnglog;
        change_logs.push_back(chnglog);
        MappingLog mapplog;
        mapping_logs.push_back(mapplog);
        locks.emplace_back();
        table_access[table_name] = -1;

        return 0;
    }

    catch (int x) {
        return -1;
    }
}

int execute_insert(string table_name, vector<string> column_val_list) {
    try {

        if(table_name_to_id.find(table_name) == table_name_to_id.end()) 
            return C_TABLE_NOT_FOUND;

        int table_id = table_name_to_id[table_name];
        Table* tbl  = tables[table_id];
        Schema* schema = tbl->schema;

        Table_Row* new_row = new Table_Row();
        int uid = UIds[table_id]++;

        Entry entry;
        entry.int_val = uid;
        new_row->fields.push_back(entry);
        
        for(int i=1;i<schema->numColumns;i++)
        {
            Entry entry;
            int len;
            switch(schema->columns[i]->type) {
                case VARCHAR:
                    entry.str_val = new string(column_val_list[i-1]);
                    new_row->fields.push_back(entry);
                    break;
                case INT:
                    entry.int_val = stoi(column_val_list[i-1]);
                    new_row->fields.push_back(entry);
                    break;
                case DOUBLE:
                    len = column_val_list.size();
                    entry.float_val = stof(column_val_list[i-1]);
                    // cout << "out of stof" << endl
                    new_row->fields.push_back(entry);
                    break;
                default:
                    break;
            }
        }

        if(!passes_pk_constraints(table_name, new_row)) {
            for (int j = 0; j < schema->numColumns; j++) {
                if (schema->columns[j]->type == VARCHAR) {
                    delete new_row->fields[j].str_val;
                }
            }
            delete new_row;
            return -1;
        }
                
        for(int i=1;i<schema->numColumns;i++)
        {
            if(!passes_range_constraints(table_name, i, column_val_list[i-1])) {
                for (int j = 0; j < schema->numColumns; j++) {
                    if (schema->columns[j]->type == VARCHAR) {
                        delete new_row->fields[j].str_val;
                    }
                }
                delete new_row;
                return -1;
            }
        }

        Log_Entry log_entry;
        log_entry.old_value = NULL;
        log_entry.new_value = new_row;
        log_entry.change_type = _INSERT;
        change_logs[table_id][uid] = log_entry;
        return 0;
    }

    catch (int x) {
        return -1;
    }
}

int execute_delete(string table_name, CondAST* cond_tree) {
    try {
        if(table_name_to_id.find(table_name) == table_name_to_id.end()) 
            return C_TABLE_NOT_FOUND;

        int table_id = table_name_to_id[table_name];
        Table* tbl = tables[table_id];

        Temp_Table* result = new Temp_Table();
        vector<int> *rids = new vector<int>();
        int ret = execute_select(result, vector<string> (1, table_name), vector<string> (1, "*"), cond_tree, rids);
        if (ret != C_OK) {
            delete result;
            delete rids;
            return ret;
        }
        
        for (int i = 0; i < result->rows.size(); i++)
        {
            ChangeLog &change_log = change_logs[table_id];
            int unique_id = result->rows[i]->fields[0].int_val;
            if (rids->at(i) != -1) {
                mapping_logs[table_id][unique_id] = rids->at(i);
            }
            if (change_log.find(unique_id) != change_log.end()) {
                delete change_log[unique_id].new_value;
                change_log[unique_id].new_value = NULL;
                change_log[unique_id].change_type = _DELETE;

                if (change_log[unique_id].old_value == NULL)
                    change_log.erase(unique_id);
            }
            else {
                Log_Entry* log_entry = new Log_Entry();
                log_entry->old_value = new Table_Row();
                *(log_entry->old_value) = *(result->rows[i]);
                log_entry->new_value = NULL;
                log_entry->change_type = _DELETE;
                change_log[unique_id] = *log_entry;
                delete log_entry;
            }
        }
        delete result;
        delete rids;
        return C_OK;
    }
    catch (int x) {
        return -1;
    }
}