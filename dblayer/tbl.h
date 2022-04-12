#ifndef _TBL_H_
#define _TBL_H_
#include <stdbool.h>

#define VARCHAR 1
#define INT     2
#define LONG    3

typedef char byte;

typedef struct {
    char *name;
    int  type;  // one of VARCHAR, INT, LONG
} ColumnDesc;

typedef struct {
    int numColumns;
    ColumnDesc **columns; // array of column descriptors
} Schema;

typedef struct {
    Schema *schema;

    int fd;         // file descriptor of associated file 
    int *lastPage;   // last page number in file
    char **pagebuf; // last page buffer
    int numPages;   // number of pages in file
    
} Table ;

typedef struct {
    int numSlots;                       // number of slots in page
    unsigned short freeSlotOffset;      // offset of next free slot
    unsigned short slotOffsets[];       // array of slot offsets
} Header ;

typedef int RecId;

int
Table_Open(char *fname, Schema *schema, bool overwrite, Table **table);

int
Table_Insert(Table *t, byte *record, int len, RecId *rid);

int
Table_Get(Table *t, RecId rid, byte *record, int maxlen);

void
Table_Close(Table *);

typedef void (*ReadFunc)(void *callbackObj, RecId rid, byte *row, int len);

void
Table_Scan(Table *tbl, void *callbackObj, ReadFunc callbackfn);

#endif
