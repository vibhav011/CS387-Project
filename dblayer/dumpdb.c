#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codec.h"
#include "tbl.h"
#include "util.h"
#include "../pflayer/pf.h"
#include "../amlayer/am.h"
#define checkerr(err)        \
    {                        \
        if (err < 0)         \
        {                    \
            PF_PrintError(); \
            exit(1);         \
        }                    \
    }

void printRow(void *callbackObj, RecId rid, byte *row, int len)
{
    Schema *schema = (Schema *)callbackObj;
    byte *cursor = row;

    for (int i = 0; i < schema->numColumns; i++)
    {
        // decoding bytes from record for each type of column and printing them
        switch (schema->columns[i]->type)
        {
        case INT:;
            int int_field = DecodeInt(cursor);
            cursor += 4; // cursor offset by size of int
            if (i + 1 != schema->numColumns)
                printf("%d,", int_field);
            else
                printf("%d\n", int_field);
            break;

        case VARCHAR:;
            char string_field[256];
            int len = DecodeCString(cursor, string_field, 256); // check max len
            cursor += 2 + len; // cursor offset by 2 byte len + string length
            if (i + 1 != schema->numColumns)
                printf("%s,", string_field);
            else
                printf("%s\n", string_field);
            break;

        case LONG:;
            long long long_field = DecodeLong(cursor);
            cursor += 8; // cursor offset by size of long long
            if (i + 1 != schema->numColumns)
                printf("%lld,", long_field);
            else
                printf("%lld\n", long_field);
            break;

        default:
            break;
        }
    }

}

#define DB_NAME "data.db"
#define INDEX_NAME "data.db.0"

void index_scan(Table *tbl, Schema *schema, int indexFD, int op, int value)
{
    int scanDesc = AM_OpenIndexScan(indexFD, 'i', 4, op, (char *)&value);
    checkerr(scanDesc);
    int recId;
    byte *record = malloc(PF_PAGE_SIZE);

    while ((recId = AM_FindNextEntry(scanDesc)) >= 0)
    {   
        int len = Table_Get(tbl, recId, record, PF_PAGE_SIZE);
        printRow(schema, recId, record, len);
    }
    free(record);
    int errVal = AM_CloseIndexScan(scanDesc);
    checkerr(errVal);
}

int main(int argc, char **argv)
{
    char *schemaTxt = "Country:varchar,Capital:varchar,Population:int";
    Schema *schema = parseSchema(schemaTxt);
    Table *tbl;

    PF_Init();          // Initialize the pflayer
    int err, fd;
    // checking if database exists
    fd = PF_OpenFile(DB_NAME);
    if(fd < 0) {
        printf("database does not exist \n");
        return -1;
    }
    PF_CloseFile(fd);
    // opening database table if it exists
    err = Table_Open(DB_NAME, schema, false, &tbl);
    checkerr(err);

    if (argc == 2 && *(argv[1]) == 's')
    {
        Table_Scan(tbl, schema, printRow);
        // invoke Table_Scan with printRow, which will be invoked for each row in the table.
    }
    else
    {
        // index scan by default
        int indexFD = PF_OpenFile(INDEX_NAME);
        checkerr(indexFD);

        if(argc == 4) {
            if (strcmp("EQUAL", argv[2]) == 0){
                index_scan(tbl, schema, indexFD, EQUAL, atoi(argv[3]));
            }
            else if (strcmp("LESS_THAN", argv[2]) == 0) {
                index_scan(tbl, schema, indexFD, LESS_THAN, atoi(argv[3]));
            }
            else if (strcmp("GREATER_THAN", argv[2]) == 0) {
                index_scan(tbl, schema, indexFD, GREATER_THAN, atoi(argv[3]));
            }
            else if (strcmp("LESS_THAN_EQUAL", argv[2]) == 0) {
                index_scan(tbl, schema, indexFD, LESS_THAN_EQUAL, atoi(argv[3]));
            }
            else if (strcmp("GREATER_THAN_EQUAL", argv[2]) == 0) {
                index_scan(tbl, schema, indexFD, GREATER_THAN_EQUAL, atoi(argv[3]));
            }
            else {index_scan(tbl, schema, indexFD, LESS_THAN_EQUAL, atoi(argv[3]));}
            
        } else {
            // Ask for populations less than 100000, then more than 100000. Together they should
            // yield the complete database.
            index_scan(tbl, schema, indexFD, LESS_THAN_EQUAL, 100000);
            index_scan(tbl, schema, indexFD, GREATER_THAN, 100000);
        }
    }
    Table_Close(tbl);
}
