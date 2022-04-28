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
using namespace std;

extern vector<ChangeLog> change_logs;
extern vector<MappingLog> mapping_logs;
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
        {
            int len = (*tr->fields[i].str_val).length();
            cout << "encoding " << len << endl;
            cout << "idx " << i << endl;
            if(pos + len + 2 > MAX_PAGE_SIZE){
                printf("insert error: no space left for filling fields in record\n");
                return C_ERROR;
            }
            strcpy(cpy, (*tr->fields[i].str_val).c_str());
            bytes_added = EncodeCString(cpy, record+pos, 257);
            cout << "bytes added " << bytes_added << endl;
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
    cout << "err: " << err << endl;
    cout<<"inserted rec is at rid: "<<rid<<endl;
    if(err != 0) return C_ERROR;

    return C_OK;

}

int execute_commit(vector<int>* ChangeIndices) {
    // TODO: Dump the change logs to disk
    int user_id = 0;
    string folder_name = "./"+to_string(user_id);
    mkdir(folder_name.c_str(), 0);
    
    for (int i = 0; i < ChangeIndices->size(); i++)
    {
        string clog_filename = folder_name+"/"+tables[ChangeIndices->at(i)]->name+".clog";
        string mlog_filename = folder_name+"/"+tables[ChangeIndices->at(i)]->name+".mlog";
        dump_clog(tables[ChangeIndices->at(i)], change_logs[ChangeIndices->at(i)], clog_filename);
        dump_mlog(tables[ChangeIndices->at(i)], mapping_logs[ChangeIndices->at(i)], mlog_filename);
    }
    

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        for (int j = 0; j < tbl->schema->numColumns; j++) {
            if (tbl->schema->columns[j]->type == VARCHAR) cout << "VARCHAR" << endl;
            else cout << "INT" << endl;
            cout << tbl->schema->columns[j]->name << endl;
        }
        ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];

        for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
            int unique_id = it->first;
            cout<<unique_id<<endl;
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

        Table_Close(tbl);
    }
    // TODO: Delete the change logs and mapping logs from disk
    for (int i = 0; i < ChangeIndices->size(); i++)
    {
        string clog_filename = folder_name+"/"+tables[ChangeIndices->at(i)]->name+".clog";
        string mlog_filename = folder_name+"/"+tables[ChangeIndices->at(i)]->name+".mlog";
        remove(clog_filename.c_str());
        remove(mlog_filename.c_str());
    }
    rmdir(folder_name.c_str());
    return C_OK;
}

int commit_delete(Table *tbl, RecId rid) {
    int ret = Table_Delete(tbl, rid);
    return ret;
}

int execute_rollback(vector<int>* ChangeIndices) {

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];

        for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
            int unique_id = it->first;
            cout<<unique_id<<endl;
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
        Table_Close(tbl);
    }
    return C_OK;
}

