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
extern map<string, int> table_name_to_id;
extern vector<Table*> tables;
extern vector<int> UIds;

#define MAX_PAGE_SIZE 4000

int commit_insert(Table *tbl, Table_Row *tr, RecId* rid) {
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

    // break into phantom_table_insert followed by append_insert_to_file followed by real_table_insert
    int err = Table_Insert(tbl, record, pos, rid);
    if(err != 0) return C_ERROR;

    return C_OK;

}

int execute_commit(vector<string> change_tables) {

    vector<int> *ChangeIndices = new vector<int>();
    for(auto table: change_tables)
        ChangeIndices->push_back(table_name_to_id[table]);

    string folder_path = "./data/" + gen_random(10) + ".log";
    filesystem::create_directory(folder_path);
    
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
        outfile.open("./data/"+tbl->name+".scm", std::ios_base::app); // append instead of overwrite
        outfile << endl << UIds[ChangeIndices->at(i)];

        PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
        PF_CloseFile(tbl->fd);
        free(tbl->lastPage);
        free(tbl->pagebuf);
        Table_Open(&("./data/"+tbl->name+".tbl")[0], tbl->schema, false, &tbl);
        change_log.clear();
        mapping_log.clear();
    }
    
    filesystem::remove_all(folder_path);
    return C_OK;
}

int commit_delete(Table *tbl, RecId rid) {
    int ret = Table_Delete(tbl, rid);
    return ret;
}

int execute_rollback_single(Table *tbl, ChangeLog& change_log, MappingLog& mapping_log) {
    cout<<"yahan?"<<endl;
    for (ChangeLog::iterator it = change_log.begin(); it != change_log.end(); it++) {
        cout<<"in"<<endl;
        int unique_id = it->first;
        Log_Entry& log_entry = it->second;
        Table_Row *old_value = log_entry.old_value;
        Table_Row *new_value = log_entry.new_value;
        cout<<log_entry.change_type<<endl;
        cout<<_UPDATE<<endl;
        cout<<_INSERT<<endl;
        cout<<_DELETE<<endl;
        switch (log_entry.change_type) {
        case _UPDATE: {
            cout<<"update"<<endl;
            int ret_value = commit_delete(tbl, mapping_log[new_value->fields[0].int_val]);
            if (ret_value != C_OK) return ret_value;
            RecId x;
            ret_value = commit_insert(tbl, old_value, &x);
            if (ret_value != C_OK) return ret_value;
            break;
        }
        case _INSERT: {
            cout<<"reverting insert on rid:"<<endl;
            cout<<mapping_log[new_value->fields[0].int_val]<<endl;
            int ret_value = commit_delete(tbl, mapping_log[new_value->fields[0].int_val]);
            if (ret_value != C_OK) return ret_value;
            break;
        }
        case _DELETE: {
            cout<<"delete"<<endl;
            RecId x;
            int ret_value = commit_insert(tbl, old_value, &x);
            if (ret_value != C_OK) return ret_value;
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
    Table_Open(&("./data/"+tbl->name+".tbl")[0], tbl->schema, false, &tbl);
    change_log.clear();
    mapping_log.clear();
    return C_OK;
}

int execute_rollback(vector<string> changed_tables) {

    vector<int> *ChangeIndices = new vector<int>();
    for(auto table: changed_tables)
        ChangeIndices->push_back(table_name_to_id[table]);

    for (int i = 0; i < ChangeIndices->size(); i++) {
        Table *tbl = tables[ChangeIndices->at(i)];
        ChangeLog& change_log = change_logs[ChangeIndices->at(i)];
        MappingLog& mapping_log = mapping_logs[ChangeIndices->at(i)];

        int ret_val = execute_rollback_single(tbl, change_log, mapping_log);

        if (ret_val != C_OK) return ret_val;
    }
    return C_OK;
}

