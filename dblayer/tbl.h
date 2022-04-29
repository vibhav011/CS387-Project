#ifndef _TBL_H_
#define _TBL_H_
#include <stdbool.h>
#include<string>
#include<string.h>
#include<vector>
#include<map>
#include<iostream>
using namespace std;

#define VARCHAR 1
#define INT     2
#define LONG    3
#define DOUBLE  4

typedef char Byte;

typedef union {
    int int_val;
    string* str_val;
    double float_val;
} Value;

struct ColumnDesc {
    char *name;
    int type;  // one of VARCHAR, INT, LONG, DOUBLE
    Value lower_bound;
    Value upper_bound;
    bool range;

    ColumnDesc(char *name, int type, Value lower_bound, Value upper_bound)
    {
        this->name = name;
        this->type = type;
        this->lower_bound = lower_bound;
        this->upper_bound = upper_bound;
        this->range = true;
    }

    ColumnDesc(char *name, int type)
    {
        this->name = name;
        this->type = type;
        this->range = false;
    }
};

struct Schema {
    int numColumns;
    ColumnDesc **columns; // array of column descriptors
    string table_name;

    Schema(int numColumns, ColumnDesc** columns, string name="")
    {
        this->numColumns = numColumns;
        this->columns = columns;
        this->table_name = name;
    }
    ~Schema()
    {
        for (int i = 0; i < numColumns; i++) {
            delete columns[i]->name;
            delete columns[i];
        }
        delete[] columns;
    }

    int getColumnNum(const char* name) {
        for(int i = 0; i<numColumns; i++) {
            if(strcmp(columns[i]->name, name) == 0) return i;
        }
        return -1;
    }
};

struct Table {
    Schema *schema;
    vector<string> pk;

    ~Table() {
        delete schema;
    }

    string name;     // name of the table
    int fd;          // file descriptor of associated file 
    int *lastPage;   // last page number in file
    char **pagebuf;  // last page buffer
    int numPages;    // number of pages in file
    
};

typedef struct {
    int numSlots;                       // number of slots in page
    int numDeletedSlots;                   // number of slots containing deleted records
    unsigned short freeSlotOffset;      // offset of next free slot
    unsigned short slotOffsets[];       // array of slot offsets
} Header ;

typedef int RecId;

int
Table_Open(char *fname, Schema *schema, bool overwrite, Table **table);

int
Table_Insert(Table *t, Byte *record, int len, RecId *rid);

int
Table_Get(Table *t, RecId rid, Byte *record, int maxlen);

int
Table_Delete(Table *t, RecId rid);

void
Table_Close(Table *);

typedef int (*ReadFunc)(void *callbackObj, RecId rid, Byte *row, int len);

void
Table_Scan(Table *tbl, void *callbackObj, ReadFunc callbackfn);

#endif
