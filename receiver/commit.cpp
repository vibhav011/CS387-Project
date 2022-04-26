#include "query.h"
#include "commit.h"
#include "string.h"
#include "../dblayer/tbl.h"
#include "../pflayer/pf.h"
#include "../dblayer/codec.h"
#include "../utils.h"

extern vector<ChangeLog> ChangeLogs;
extern vector<MappingLog> MappingLogs;
extern vector<Table*> tables;

#define MAX_PAGE_SIZE 4000

int commit_insert(Table *tbl, Table_Row *tr){
    int numfields = tbl->schema->numColumns;
    char record[MAX_PAGE_SIZE];

    int pos = 0, bytes_added = 0;
    char cpy[256]; // better solution possible

    for(int i=0; i<tbl->schema->numColumns; i++){
        switch (tbl->schema->columns[i]->type)
        {
        case VARCHAR:
            int len = (*tr->fields[i].str_val).length();
            if(pos + len + 2 > MAX_PAGE_SIZE){
                printf("insert error: no space left for filling fields in record\n");
                return C_ERROR;
            }
            strcpy(cpy, (*tr->fields[i].str_val).c_str());
            bytes_added = EncodeCString(cpy, record+pos, 257);
            pos += bytes_added;
            break;

        case INT:
            if(pos+4>MAX_PAGE_SIZE){
                printf("insert error: no space left for int\n");
                return C_ERROR;
            }   
            bytes_added = EncodeInt(tr->fields[i].int_val, record+pos);
            pos += bytes_added;
            break;
        
        case DOUBLE:
            if(pos+8>MAX_PAGE_SIZE){
                printf("insert error: no space left for double/float value");
                return C_ERROR;
            }
            bytes_added = EncodeDouble(tr->fields[i].float_val, record+pos);
            pos += bytes_added;
            break;

        default:
            printf("insert error: we should not reach here\n");
            break;
        }
    }

    RecId rid;
    int err = Table_Insert(tbl, record, pos, &rid);
    
    if(err != 0) return C_ERROR;

    return C_OK;

}

int execute_commit(vector<int>* ChangeIndices) {
    // TODO: Dump the change logs to disk

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = ChangeLogs[ChangeIndices->at(i)];
        MappingLog& mapping_log = MappingLogs[ChangeIndices->at(i)];

        for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
            int unique_id = it->first;
            Log_entry& log_entry = it->second;
            Table_Row *old_value = log_entry.old_value;
            Table_Row *new_value = log_entry.new_value;

            switch (log_entry.change_type) {
            case UPDATE: {
                int ret_value = commit_delete(tbl, mapping_log[old_value->fields[0].int_val]);
                if (ret_value != C_OK) return ret_value;
                ret_value = commit_insert(tbl, new_value);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            case INSERT: {
                int ret_value = commit_insert(tbl, new_value);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            case DELETE: {
                int ret_value = commit_delete(tbl, mapping_log[old_value->fields[0].int_val]);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            default:
                break;
            }
        }
    }
    // TODO: Delete the change logs and mapping logs from disk
    return C_OK;
}

int commit_delete(Table *tbl, RecId rid) {
    return C_OK;
}