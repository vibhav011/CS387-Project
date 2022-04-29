#include <string.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <list>
#include "utils.h"
#include "./receiver/commit.h"
#include "receiver/helper.h"
#include "dblayer/util.h"

using namespace std;

extern map<string, int> table_name_to_id;
extern vector<Table*> tables;              // objects of all tables
extern vector<int> UIds;                   // constanstly increasing uids for each of the tables
extern vector<ChangeLog> change_logs;     // objects of change logs for corresponding tables in `tables`
extern vector<MappingLog> mapping_logs;
extern list<mutex> locks;
extern map<string, int> table_access;

int obtain_write_lock(int worker_id, string table_name){
    map<string, int>::iterator it;

    if((it = table_name_to_id.find(table_name)) == table_name_to_id.end()){
        return C_TABLE_NOT_FOUND;
    } 
    
    if(table_access[table_name] != worker_id){
        list<std::mutex>::iterator it2 = locks.begin();
        advance(it2, it->second);
        it2->lock();
        table_access[table_name] = worker_id;
    }

    return C_OK;
}

int release_write_lock(string table_name){
    map<string, int>::iterator it;

    if((it = table_name_to_id.find(table_name)) == table_name_to_id.end()){
        return C_TABLE_NOT_FOUND;
    }
    table_access[table_name] = -1;

    list<std::mutex>::iterator it2 = locks.begin();
    advance(it2, it->second);
    it2->unlock();

    return C_OK;
}

void recover_from_folder(string folder_name) {
    map<int, ChangeLog> change_logs;
    map<int, MappingLog> mapping_logs;
    for (const auto& dirEntry : std::filesystem::directory_iterator(folder_name)) {
        if (!dirEntry.is_directory()) {
            string s = dirEntry.path().filename();
            if (s.size() > 5) {
                if (s.substr(s.size() - 5) == ".clog") {
                    string clog_path = dirEntry.path().string();
                    string table_name = s.substr(0, s.size() - 5);
                    read_clog(change_logs[table_name_to_id[table_name]], clog_path);
                }
                else if (s.substr(s.size() - 5) == ".mlog") {
                    string mlog_path = dirEntry.path().string();
                    string table_name = s.substr(0, s.size() - 5);
                    read_mlog(mapping_logs[table_name_to_id[table_name]], mlog_path);
                }
            }
        }
    }
    for (auto it = change_logs.begin(); it != change_logs.end(); it++) {
        int table_id = it->first;
        ChangeLog change_log = it->second;

        if (mapping_logs.find(table_id) == mapping_logs.end())
            continue;

        MappingLog mapping_log = mapping_logs[table_id];
        // TODO: implement following functions and uncomment
        cout<<"called exec rb single on "<<table_id<<endl;
        execute_rollback_single(tables[table_id], change_log, mapping_log);
    }
}

void setup_and_recover() {
    vector<string> log_folders;
    vector<string> table_names;

    for (const auto& dirEntry : std::filesystem::directory_iterator("./data")) {
        cout<<dirEntry.path().filename()<<endl;
        if (dirEntry.is_directory()) {
            string s = dirEntry.path(); // .filename();
            if (s.size() > 4) {
                if (s.substr(s.size() - 4) == ".log") {
                    cout<<"s "<<s<<endl;
                    log_folders.push_back(s);
                }
            }
        }
        else {
            string s = dirEntry.path().filename();
            if (s.size() > 4) {
                if (s.substr(s.size() - 4) == ".tbl") {
                    string tbl_name = s.substr(0, s.size() - 4);
                    table_names.push_back(tbl_name);
                }
            }
        }
    }

    for (string tbl_name: table_names) {
        string tbl_path = "./data/" + tbl_name + ".tbl";
        string tbl_scm_path = "./data/" + tbl_name + ".scm";
        
        if (!std::filesystem::exists(tbl_scm_path)) {
            std::filesystem::remove(tbl_path);
            std::filesystem::remove(tbl_scm_path);
            continue;
        }

        ifstream schema_file; 
        schema_file.open(tbl_scm_path);
        char discard[256];
        schema_file.getline(discard, 256);
        int num_pk = 0;
        schema_file >> num_pk;
        vector<string> pk;
        for (int i = 0; i < num_pk; i++) {
            string pk_name;
            schema_file >> pk_name;
            pk.push_back(pk_name);
        }

        Schema* schema = parseSchema(discard);
        schema->columns[0]->range = false;
        for (int i = 1; i < schema->numColumns; i++)
        {
            bool range;
            int int_val;
            float float_val;
            string* str_val;
            schema_file >> range;
            schema->columns[i]->range = range;
            if(range)
            {
                switch(schema->columns[i]->type) {
                    case INT:
                        schema_file >> int_val;
                        schema->columns[i]->lower_bound.int_val = int_val;
                        schema_file >> int_val;
                        schema->columns[i]->upper_bound.int_val = int_val;
                        break;
                    case DOUBLE:
                        schema_file >> float_val;
                        schema->columns[i]->lower_bound.float_val = float_val;
                        schema_file >> float_val;
                        schema->columns[i]->upper_bound.float_val = float_val;
                        break;
                    case VARCHAR:
                        break;
                }
            }
        }
        
        schema->table_name = tbl_name;
        Table *table = new Table();
        Table_Open((char *)tbl_path.c_str(), schema, false, &table);
        table->name = tbl_name;
        table->pk = pk;

        int uid = 0;
        while (!schema_file.eof())
            schema_file >> uid;

        table_name_to_id[tbl_name] = tables.size();
        table_access[table->name] = -1; // initially nobody accessing the table

        tables.push_back(table);
        UIds.push_back(uid);
        change_logs.push_back(ChangeLog());
        mapping_logs.push_back(MappingLog());
        
        // instantiating locks
        locks.emplace_back();

        schema_file.close();
    }

    for (string s: log_folders) {
        cout<<"rolling back "<<s<<endl;
        recover_from_folder(s);
        for (const auto& de: std::filesystem::directory_iterator(s)) {
            std::filesystem::remove(de.path());
        }
        std::filesystem::remove(s);
    }
    // instantiate_locks();
}