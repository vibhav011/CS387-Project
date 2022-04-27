#include "helper.h"
#include <fstream>
using namespace std;

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

}