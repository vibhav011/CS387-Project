#include<string>
#include<vector>
#include<iostream>
using namespace std;

union field_type
{
    int int_val;
    double double_val;
    string string_val;
};

struct Schema
{
    int num_cols;
    vector<string> col_names;
    int *dts;
};

struct Table
{
    Schema *schema;
    vector<vector<string> > table_data;
};