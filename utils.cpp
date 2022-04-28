#include "utils.h"

void checkerr(int err_code) {
    switch(err_code) {
        case C_OK:
            cout<<"successfully terminated"<<endl;
            break;
        case C_TRUE:
            cout<<"true output"<<endl;
            break;
        case C_FALSE:
            cout<<"false output"<<endl;
            break;
        case C_ERROR:
            cout<<"error"<<endl;
            break;
        case C_TABLE_NOT_FOUND:
            cout<<"table not found"<<endl;
            break;
        case C_FIELD_NOT_FOUND:
            cout<<"field not found"<<endl;
            break;
    }
}


map<string, int> table_name_to_id;
vector<Table*> tables;              // objects of all tables
vector<int> UIds;                   // constanstly increasing uids for each of the tables
vector<ChangeLog> change_logs;     // objects of change logs for corresponding tables in `tables`
vector<MappingLog> mapping_logs;

Table_Row::Table_Row()
{
    this->num_fields = 0;
}

Entry Table_Row::getField(int ind)
{
    return this->fields[ind];
}

Log_Entry::Log_Entry()
{
    this->old_value = NULL;
    this->new_value = NULL;
}

Log_Entry::~Log_Entry()
{
    delete this->old_value;
    delete this->new_value;
}
