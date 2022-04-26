#include "query.h"
#include "commit.h"
#include "../dblayer/tbl.h"
#include "../pflayer/pf.h"

extern vector<ChangeLog> ChangeLogs;
extern vector<MappingLog> MappingLogs;
extern vector<Table*> tables;

int execute_commit(vector<int>* ChangeIndices) {
    // TODO: Dump the change logs to disk

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = ChangeLogs[ChangeIndices->at(i)];
        MappingLog& mapping_log = MappingLogs[ChangeIndices->at(i)];

        for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
            int unique_id = it->first;
            Log_entry& log_entry = it->second;
            Table_row *old_value = log_entry.old_value;
            Table_row *new_value = log_entry.new_value;

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
    
}