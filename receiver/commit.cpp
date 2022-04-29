#include "query.h"
#include "commit.h"
#include "string.h"
#include "../dblayer/tbl.h"
#include "../pflayer/pf.h"
#include "../dblayer/codec.h"
#include "../utils.h"
#include "helper.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
using namespace std;

extern vector<ChangeLog> change_logs;
extern vector<MappingLog> mapping_logs;
extern vector<Table*> tables;
extern vector<int> UIds;

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
        {
            int len = (*tr->fields[i].str_val).length();
            if(pos + len + 2 > MAX_PAGE_SIZE){
                printf("insert error: no space left for filling fields in record\n");
                return C_ERROR;
            }
            strcpy(cpy, (*tr->fields[i].str_val).c_str());
            bytes_added = EncodeCString(cpy, record+pos, 257);
            pos += bytes_added;
            break;
        }
        case INT:
        {
            if(pos+4>MAX_PAGE_SIZE){
                printf("insert error: no space left for int\n");
                return C_ERROR;
            }   
            bytes_added = EncodeInt(tr->fields[i].int_val, record+pos);
            pos += bytes_added;
            break;
        }
        case DOUBLE:
        {
            if(pos+8>MAX_PAGE_SIZE){
                printf("insert error: no space left for double/float value");
                return C_ERROR;
            }
            bytes_added = EncodeDouble(tr->fields[i].float_val, record+pos);
            pos += bytes_added;
            break;
        }
        default:
        {
            printf("insert error: we should not reach here\n");
            break;
        }
        }
    }

    RecId rid;
    // break into phantom_table_insert followed by append_insert_to_file followed by real_table_insert
    int err = Table_Insert(tbl, record, pos, &rid);
    if(err != 0) return C_ERROR;

    return C_OK;

}

int execute_commit(vector<int>* ChangeIndices) {
    string folder_path = "./data/" + gen_random(10) + ".log";
    std::filesystem::create_directory(folder_path);
    
    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];

        dump_clog(tbl, change_log, folder_path+"/"+tbl->name+".clog");
        dump_mlog(tbl, mapping_log, folder_path+"/"+tbl->name+".mlog");

        for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
            int unique_id = it->first;
            Log_Entry& log_entry = it->second;
            Table_Row *old_value = log_entry.old_value;
            Table_Row *new_value = log_entry.new_value;

            switch (log_entry.change_type) {
            case _UPDATE: {
                int ret_value = commit_delete(tbl, mapping_log[old_value->fields[0].int_val]);
                if (ret_value != C_OK) return ret_value;
                ret_value = commit_insert(tbl, new_value);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            case _INSERT: {
                int ret_value = commit_insert(tbl, new_value);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            case _DELETE: {
                int ret_value = commit_delete(tbl, mapping_log[old_value->fields[0].int_val]);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            default:
                break;
            }
        }
        std::ofstream outfile;
        outfile.open("./data/"+tbl->name+".scm", std::ios_base::app); // append instead of overwrite
        outfile << endl << UIds[ChangeIndices->at(i)];

        if (*tbl->lastPage != -1) {
            PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
            PF_GetThisPage(tbl->fd, *tbl->lastPage, tbl->pagebuf);
        }
        change_log.clear();
        mapping_log.clear();
    }
    
    std::filesystem::remove_all(folder_path);
    return C_OK;
}

int commit_delete(Table *tbl, RecId rid) {
    int ret = Table_Delete(tbl, rid);
    return ret;
}

int execute_rollback_single(Table *tbl, ChangeLog& change_log, MappingLog& mapping_log) {
    for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
        int unique_id = it->first;
        Log_Entry& log_entry = it->second;
        Table_Row *old_value = log_entry.old_value;
        Table_Row *new_value = log_entry.new_value;

        switch (log_entry.change_type) {
        case _UPDATE: {
            int ret_value = commit_delete(tbl, mapping_log[new_value->fields[0].int_val]);
            if (ret_value != C_OK) return ret_value;
            ret_value = commit_insert(tbl, old_value);
            if (ret_value != C_OK) return ret_value;
            break;
        }
        case _INSERT: {
            int ret_value = commit_delete(tbl, mapping_log[new_value->fields[0].int_val]);
            if (ret_value != C_OK) return ret_value;
            break;
        }
        case _DELETE: {
            int ret_value = commit_insert(tbl, old_value);
            if (ret_value != C_OK) return ret_value;
            break;
        }
        default:
            break;
        }
    }
    return C_OK;
}

int execute_rollback(vector<int>* ChangeIndices) {

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];

        int ret_val = execute_rollback_single(tbl, change_log, mapping_log);
        Table_Close(tbl);
        if (ret_val != C_OK) return ret_val;
    }
    return C_OK;
}

