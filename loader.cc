#include <string.h>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
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
        schema->table_name = tbl_name;
        Table *table = new Table();
        Table_Open((char *)tbl_path.c_str(), schema, false, &table);
        table->name = tbl_name;
        table->pk = pk;

        int uid = 0;
        while (!schema_file.eof())
            schema_file >> uid;

        table_name_to_id[tbl_name] = tables.size();
        tables.push_back(table);
        UIds.push_back(uid);
        change_logs.push_back(ChangeLog());
        mapping_logs.push_back(MappingLog());
        
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

}