#ifndef _QRY_H_
#define _QRY_H_
#include "../dblayer/tbl.h"
#include "../ast.h"
#include "../utils.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <iterator>
#include <algorithm>
using namespace std;

struct Temp_Table {
    string name;
    Schema* schema;
    vector<Table_Row*> rows;

    Temp_Table(){
        this->schema = NULL;
    }

    Temp_Table(Schema* schema) {
        this->schema = schema;
    }

    ~Temp_Table() {
        for (int i = 0; i < rows.size(); i++)
            delete rows[i];
    }

    void prettyPrint()
    {
        if(this->schema == NULL)
        {
            cout<<"Empty table"<<endl;
            return ;
        }
        vector<int> types;
        cout<<"Number of columns: "<<this->schema->numColumns<<endl;
        cout<<"Number of rows: "<<this->rows.size()<<endl;
        for(int i=0;i<this->schema->numColumns;i++)
        {
            types.push_back(this->schema->columns[i]->type);
            cout<<setw(20)<<this->schema->columns[i]->name;
        }
        cout<<endl;

        for(int ii = 0; ii < this->rows.size(); ii++)
        {
            assert(rows[ii]->fields.size() == this->schema->numColumns);
            for(int i=0;i<this->schema->numColumns;i++)
            {
                if(types[i] == INT)
                    cout<<setw(20)<<rows[ii]->getField(i).int_val;
                else if(types[i] == DOUBLE)
                    cout<<setw(20)<<rows[ii]->getField(i).float_val;
                else
                    cout<<setw(20)<<*(rows[ii]->getField(i).str_val);
            }
            cout<<endl;
        }
    }

    void set_schema_columns(vector<string> names)
    {
        for(int i=0;i<names.size();i++)
            this->schema->columns[i]->name = &(names[i])[0];
    }

    void set_name(string name)
    {
        this->name = name;
    }
};


struct Query_Obj {
    vector<string> col_names;
    CondAST* cond_tree;
    Temp_Table *temp_table;
    int tbl1_id, tbl2_id;
    Table_Row *tr1, *tr2;
    int ret_value;

    Query_Obj(vector<string>, CondAST*, Temp_Table*, int, int);
};

struct Update_Pair {
    string lhs;
    string rhs;

    Update_Pair(string lhs, string rhs)
    {
        this->lhs = lhs;
        this->rhs = rhs;
    }
};

typedef vector<Temp_Table*> table_list;

int execute_create_temp(table_list tables){return C_OK;};
int execute_select(Temp_Table *result, vector<string> table_names, vector<string> col_names, CondAST *cond_tree=NULL);
int execute_update(string table_name, vector<Update_Pair*> update_list, CondAST* cond_tree=NULL);
int execute_create(string table_name, vector<ColumnDesc*> &column_desc_list, vector<string> constraint={});
int execute_insert(string table_name, vector<string> column_val_list);
int execute_delete(string table_name, CondAST *cond_tree=NULL);

int query_process(Query_Obj *cObj, Table_Row *tr);
int log_scan(Query_Obj *cObj);

#endif