#include "helper.h"
#include "../utils.h"
#include <fstream>
using namespace std;

// dumping float_val for each entry, read float_val in read_log as well
int dump_clog(Table *tbl, ChangeLog &change_log, string filename){
    fstream fs;
    fs.open(filename, fstream::out);

    int numcols = tbl->schema->numColumns;
    fs << tbl->name << ' ' << numcols << ' ' << change_log.size() << endl;
    for(int i=0; i<numcols; i++){
        fs << tbl->schema->columns[i]->type << ' ';
    }
    fs << endl;
    ChangeLog:: iterator p;
    int i = 0;
    for(p = change_log.begin(); p != change_log.end(); p++){
        fs << p->first << ' ' << p->second.change_type << endl;
        
        if(p->second.change_type != _INSERT){
            if (p->second.old_value == NULL) continue;

            for(int i=0; i<numcols; i++){
                switch (tbl->schema->columns[i]->type) {
                    case INT:
                        fs << p->second.old_value->fields[i].int_val << endl;
                        break;
                    case DOUBLE:
                        fs << p->second.old_value->fields[i].float_val << endl;
                        break;
                    case VARCHAR:
                        fs << *(p->second.old_value->fields[i].str_val) << endl;
                        break;
                    default:
                        break;
                }
            }
        }

        if(p->second.change_type != _DELETE){
            if (p->second.new_value == NULL) continue;
            for(int i=0; i<numcols; i++){
                switch (tbl->schema->columns[i]->type) {
                    case INT:
                        fs << p->second.new_value->fields[i].int_val << endl;
                        break;
                    case DOUBLE:
                        fs << p->second.new_value->fields[i].float_val << endl;
                        break;
                    case VARCHAR:
                        fs << *(p->second.new_value->fields[i].str_val) << endl;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    fs.close();
    return C_OK;
}

int dump_mlog(Table *tbl, MappingLog& mapping_log, string filename) {
    fstream fs;
    fs.open(filename, fstream::out);

    // int numcols = tbl->schema->numColumns;
    // fs << tbl->name << ' ' << numcols << endl;
    // for(int i=0; i<numcols; i++){
    //     fs << tbl->schema->columns[i]->type << ' ';
    // }
    // fs << endl;

    MappingLog:: iterator p;
    for(p = mapping_log.begin(); p != mapping_log.end(); p++){
        fs << p->first << " " << p->second << endl;
    }
    fs.close();
    return C_OK;
}

int read_clog(ChangeLog &change_log, string filename){
    ifstream indata; 
    int num; 
    indata.open(filename); 
    if(!indata) { 
        cerr << "Error: log file could not be opened" << endl;
        return -1;
    }
    string table_name;
    indata >> table_name;
    int numcols;
    indata >> numcols;
    int num_entries;
    indata >> num_entries;
    int scm[numcols];
    for (int i = 0; i < numcols; i++) {
        indata >> scm[i];
    }

    int uid;
    int change_type;
    char discard[256];
    cout<<"ne in read_clog "<<num_entries<<endl;
    for (int i = 0; i < num_entries; i++) {
        Log_Entry log_entry;
        Table_Row *old_row, *new_row;
        old_row = new_row = NULL;
        indata >> uid;
        indata >> change_type;
        cout<<"change type okay in read_clog? "<<change_type<<endl;
        if(change_type != _INSERT) {
            old_row = new Table_Row();
            for (int i = 0; i < numcols; i++)
            {
                indata.getline(discard, 256);
                Entry* entry = new Entry();
                switch (scm[i]) {
                    case INT:
                        entry->int_val = atoi(discard);
                        break;
                    case DOUBLE:
                        entry->float_val = atof(discard);
                        break;
                    case VARCHAR:
                        entry->str_val = new string(discard);
                        break;
                    default:
                        break;
                }

                old_row->fields.push_back(*entry);
                delete entry;  
            }
        }

        if(change_type != _DELETE) {
            new_row = new Table_Row();
            for (int i = 0; i < numcols; i++)
            {
                indata.getline(discard, 256);
                Entry* entry = new Entry();
                switch (scm[i]) {
                    case INT:
                        entry->int_val = atoi(discard);
                        break;
                    case DOUBLE:
                        entry->float_val = atof(discard);
                        break;
                    case VARCHAR:
                        entry->str_val = new string(discard);
                        break;
                    default:
                        break;
                }

                new_row->fields.push_back(*entry);
                delete entry;  
            }
        }

        log_entry.change_type = (Change_type)change_type;
        log_entry.old_value = old_row;
        log_entry.new_value = new_row;

        change_log[uid] = log_entry;
    }

    indata.close();
    return C_OK;
}

int read_mlog(MappingLog& mapping_log, string filename) {
    ifstream indata; 
    int num; 
    indata.open(filename); 
    if(!indata) { 
        cerr << "Error: log file could not be opened" << endl;
        return -1;
    }

    while ( !indata.eof() ) {
       int uid;
       int rid;
       indata >> uid;
       indata >> rid;
       mapping_log[uid] = rid;
    }
    indata.close();
    return C_OK;
}

string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}