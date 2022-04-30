#include "query.h"
#include "commit.h"
#include "string.h"
#include "../dblayer/tbl.h"
#include "../pflayer/pf.h"
#include "../dblayer/codec.h"
#include "utils.h"
#include "helper.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
using namespace std;

extern vector<ChangeLog> change_logs;
extern vector<MappingLog> mapping_logs;
extern map<string, int> table_name_to_id;
extern vector<Table*> tables;
extern vector<int> UIds;

int release_write_lock(string table_name);

#define MAX_PAGE_SIZE 4000

int commit_insert(Table *tbl, Table_Row *tr, RecId* rid, int orid=-1) {
    int numfields = tbl->schema->numColumns;
    char record[MAX_PAGE_SIZE];

    int ret = -1;
    if(orid >= 0) ret = Table_Get(tbl, orid, record, MAX_PAGE_SIZE);

    if(ret != -1){
        int uid = DecodeInt(record);
        if(uid == tr->fields[0].int_val){
            *rid = orid;
            return C_OK;
        }
    }

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

    // break into phantom_table_insert followed by append_insert_to_file followed by real_table_insert
    int err = Table_Insert(tbl, record, pos, rid);
    cout << "RID alloted " << *rid << endl;
    if(err != 0) return C_ERROR;

    return C_OK;

}

int execute_commit(set<string> &change_tables) {

    vector<int> *ChangeIndices = new vector<int>();
    for(auto table: change_tables)
        ChangeIndices->push_back(table_name_to_id[table]);

    string folder_path = DATA_PATH + gen_random(10) + ".log";
    filesystem::create_directory(folder_path);

#ifdef NOLOGS
    cout << "Crash in befpre dumping to actual database" << endl;
    exit(1);
#endif
    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];

        dump_clog(tbl, change_log, folder_path+"/"+tbl->name+".clog");
        dump_mlog(tbl, mapping_log, folder_path+"/"+tbl->name+".mlog");
        cout << "Dumped logs" << endl;

        for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
            int unique_id = it->first;
            Log_Entry& log_entry = it->second;
            Table_Row *old_value = log_entry.old_value;
            Table_Row *new_value = log_entry.new_value;

#ifdef SOMELOGS
            cout << "Crash in between dumping to actual database" << endl;
            if(it != change_log.begin()){
                exit(1);
            }
#endif

            switch (log_entry.change_type) {
            case _UPDATE: {
                int ret_value = commit_delete(tbl, mapping_log[old_value->fields[0].int_val]);
                if (ret_value != C_OK) return ret_value;
                RecId x;
                ret_value = commit_insert(tbl, new_value, &x);
                if (ret_value != C_OK) return ret_value;
                break;
            }
            case _INSERT: {
                int uid = new_value->fields[0].int_val;
                RecId rid;
                int ret_value = commit_insert(tbl, new_value, &rid);
                
                std::ofstream outfile;
                outfile.open(folder_path+"/"+tbl->name+".mlog", std::ios_base::app); // append instead of overwrite
                outfile << uid << ' ' << rid << endl;
                outfile.close();
                
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
        outfile.open(DATA_PATH+tbl->name+".scm", std::ios_base::app); // append instead of overwrite
        outfile << endl << UIds[ChangeIndices->at(i)];

        PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
        PF_CloseFile(tbl->fd);
        free(tbl->lastPage);
        free(tbl->pagebuf);
        Table_Open(&(DATA_PATH+tbl->name+".tbl")[0], tbl->schema, false, &tbl);
        change_log.clear();
        mapping_log.clear();
    }
    
    for(string name: change_tables){
        release_write_lock(name);
    }

#ifdef TOTLOGS
        cout << "Crash in after dumping to actual database but before deleting log files" << endl;
        exit(1);
#endif

    filesystem::remove_all(folder_path);
    return C_OK;
}

int commit_delete(Table *tbl, RecId rid) {
    if(rid < 0) return C_OK;
    int ret = Table_Delete(tbl, rid);
    return ret;
}

int execute_rollback_single(Table *tbl, ChangeLog& change_log, MappingLog& mapping_log) {
    int ret_value = C_OK;
    for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
        int unique_id = it->first;
        Log_Entry& log_entry = it->second;
        Table_Row *old_value = log_entry.old_value;
        Table_Row *new_value = log_entry.new_value;

        int rid = -1;

        switch (log_entry.change_type) {
        case _UPDATE: {
            if(mapping_log.find(new_value->fields[0].int_val) != mapping_log.end()){
                rid = mapping_log[new_value->fields[0].int_val];
            }
            cout<<"rollback update"<<endl;
            ret_value = commit_delete(tbl, rid);
            RecId x;
            ret_value = commit_insert(tbl, old_value, &x);
            if (ret_value != C_OK) return ret_value;
            break;
        }
        case _INSERT: {
            if(mapping_log.find(new_value->fields[0].int_val) != mapping_log.end()){
                rid = mapping_log[new_value->fields[0].int_val];
            }
            cout << "rollback insert" << endl;
            ret_value = commit_delete(tbl, rid);
            break;
        }
        case _DELETE: {
            if(mapping_log.find(old_value->fields[0].int_val) != mapping_log.end()){
                rid = mapping_log[old_value->fields[0].int_val];
            }
            cout<<"rollback delete"<<endl;
            RecId x;
            ret_value = commit_insert(tbl, old_value, &x, rid);
            break;
        }
        default:
            break;
        }
    }
    PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
    PF_CloseFile(tbl->fd);
    free(tbl->lastPage);
    free(tbl->pagebuf);
    Table_Open(&(DATA_PATH+tbl->name+".tbl")[0], tbl->schema, false, &tbl);
    cout << "clearing logs" << endl;
    change_log.clear();
    mapping_log.clear();
    return ret_value;
}

int execute_rollback(set<string> &change_tables) {
    cout << "rollback" << endl;
    vector<int> *ChangeIndices = new vector<int>();
    for(auto table: change_tables)
        ChangeIndices->push_back(table_name_to_id[table]);

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        // ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        // MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];
        cout << "single rollback" << endl;
        int ret_val = execute_rollback_single(tbl, change_logs[ChangeIndices->at(i)], mapping_logs[ChangeIndices->at(i)]);

        if (ret_val != C_OK) return ret_val;
    }
    delete ChangeIndices;

    for(string name: change_tables){
        release_write_lock(name);
    }
    cout << "exiting rollback" << endl;
    return C_OK;
}