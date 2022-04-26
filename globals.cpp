#include "utils.h"
#include <mutex> 
using namespace std;

map<string, int> table_name_to_id;
vector<Table*> tables;              // objects of all tables
vector<int> UIds;                   // constanstly increasing uids for each of the tables
vector<mutex> table_mutex;          // write mutex for each table
vector<ChangeLog> change_logs;     // objects of change logs for corresponding tables in `tables`
vector<MappingLog> mapping_logs;

string global_query;
mutex query_mutex;