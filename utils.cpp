#include "utils.h"

map<string, int> table_name_to_id;
vector<Table*> tables;              // objects of all tables
vector<int> UIds;                   // constanstly increasing uids for each of the tables
vector<ChangeLog> change_logs;     // objects of change logs for corresponding tables in `tables`
vector<MappingLog> mapping_logs;

Entry Table_Row::getField(int ind)
{
    return this->fields[ind];
}

void Table_Row::addField(Entry entry)
{
    this->fields.push_back(entry);
}

Log_Entry::Log_Entry()
{
    this->old_value = NULL;
    this->new_value = NULL;
}
