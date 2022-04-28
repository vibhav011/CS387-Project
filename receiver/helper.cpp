#include "helper.h"
#include "utils.h"
#include <fstream>
using namespace std;

extern

// dumping float_val for each entry, read float_val in read_log as well
int dump_log(Table *tbl, ChangeLog &change_log, string filename){
    fstream fs;
    fs.open(filename, fstream::out);

    int numcols = tbl->schema->numColumns;
    fs << tbl->name << ' ' << numcols << endl;
    for(int i=0; i<tbl->schema->numColumns; i++){
        fs << tbl->schema->columns[i]->type << ' ';
    }
    fs << endl;

    for(auto const &p: change_log){
        fs << p.first << ' ' << p.second.change_type << ' ';
        
        if(p.second.change_type != _INSERT){
            for(int i=0; i<numcols; i++){
                cout << p.second.old_value->fields[i].float_val << ' ';
            }
        }
        if(p.second.change_type != _DELETE){
            for(int i=0; i<numcols; i++){
                cout << p.second.new_value->fields[i].float_val << ' ';
            }
        }
    }
    fs.close();
}

int read_log(ChangeLog &change_log, string filename){

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
    char discard[256];
    indata.getline(discard, 256);
    int uid;
    int change_type;

    while ( !indata.eof() ) { 
        Log_Entry* log_entry = new Log_Entry();
        Table_Row* old_row = new Table_Row();
        Table_Row* new_row = new Table_Row();
        indata >> uid;
        indata >> change_type;
        if(change_type != _INSERT) {
            for (int i = 0; i < numcols; i++)
            {
                Entry* entry = new Entry();
                double float_val;
                indata >> float_val;
                entry->float_val = float_val;
                old_row->fields.push_back(*entry);
                delete entry;
            }
            log_entry->old_value = old_row;
        }
        else{
            log_entry->old_value = NULL; 
        }
        if(change_type != _DELETE) {
            for (int i = 0; i < numcols; i++)
            {
                Entry* entry = new Entry();
                double float_val;
                indata >> float_val;
                entry->float_val = float_val;
                new_row->fields.push_back(*entry);
                delete entry;
            }
            log_entry->new_value = new_row;
        }
        else{
            log_entry->new_value = NULL; 
        }
        change_log[uid] = *log_entry;
    }
    indata.close();
    
}