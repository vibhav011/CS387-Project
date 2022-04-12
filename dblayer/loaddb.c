#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "codec.h"
#include "../pflayer/pf.h"
#include "../amlayer/am.h"
#include "tbl.h"
#include "util.h"

#define checkerr(err)        \
    {                        \
        if (err < 0)         \
        {                    \
            PF_PrintError(); \
            exit(1);         \
        }                    \
    }

#define MAX_PAGE_SIZE 4000

#define DB_NAME "data.db"
#define INDEX_NAME "data.db.0"
#define CSV_NAME "data.csv"

/*
Takes a schema, and an array of strings (fields), and uses the functionality
in codec.c to convert strings into compact binary representations
 */
int encode(Schema *sch, char **fields, byte *record, int spaceLeft)
{
    // function for filling record by byte encoded values of the fields

    int num_bytes = 0; // to store the total number of bytes taken up by all attributes in the row
    int temp_bytes = 0; // to track num bytes returned by encode methods

    for (int i = 0; i < sch->numColumns; i++)
    {
        // cases for each of the column types in the schema
        switch (sch->columns[i]->type)
        {
        case VARCHAR:
            if (spaceLeft < strlen(fields[i])+2) // checking if space for len+str left
            {
                printf("no space left for filling fields in record\n");
                return -1;
            }
            temp_bytes = EncodeCString(fields[i], record+num_bytes, 257);
            num_bytes += temp_bytes;
            spaceLeft -= temp_bytes; // updating space left
            break;
        case INT:
            if (spaceLeft < 4)  // checking if space for int left
            {
                printf("no space left for filling fields in record\n");
                return -1;
            }
            temp_bytes = EncodeInt(atoi(fields[i]), record+num_bytes);
            num_bytes += temp_bytes;
            spaceLeft -= temp_bytes; // updating space left
            break;
        case LONG:
            if (spaceLeft < 8)  // checking if space for long left
            {
                printf("no space left for filling fields in record\n");
                return -1;
            }
            temp_bytes = EncodeLong(atoll(fields[i]), record+num_bytes);
            num_bytes += temp_bytes;
            spaceLeft -= temp_bytes; // updating space left
            break;
        }
    }
    return num_bytes;
}

Schema *
loadCSV()
{
    // Open csv file, parse schema
    FILE *fp = fopen(CSV_NAME, "r");
    if (!fp)
    {
        perror("data.csv could not be opened");
        exit(EXIT_FAILURE);
    }

    char buf[MAX_LINE_LEN];
    char *line = fgets(buf, MAX_LINE_LEN, fp);
    if (line == NULL)
    {
        fprintf(stderr, "Unable to read data.csv\n");
        exit(EXIT_FAILURE);
    }

    // Open main db file
    Schema *sch = parseSchema(line);
    Table *tbl;

    PF_Init();          // Initialize the pflayer
    int err;
    err = Table_Open(DB_NAME, sch, false, &tbl); // opening the table (creating if it doesn't exist)
    checkerr(err); // for table open errors
    AM_CreateIndex(DB_NAME, 0, 'i', 4); // creating a new index for type int, size 4 and index no. 0
    int indexFD = PF_OpenFile(INDEX_NAME); // opening file for passing the descriptor to index creation
    checkerr(indexFD);

    char *tokens[MAX_TOKENS];
    char record[MAX_PAGE_SIZE];
    
    while ((line = fgets(buf, MAX_LINE_LEN, fp)) != NULL)
    {   
        int n = split(line, ",", tokens);
        assert(n == sch->numColumns);
        int len = encode(sch, tokens, record, sizeof(record));
        RecId rid;

        checkerr(len); // for encode errors
        err = Table_Insert(tbl, record, len, &rid); // insert encoded record into table
        checkerr(err);

        printf("%d %s\n", rid, tokens[0]);

        // Indexing on the population column
        int population = atoi(tokens[2]);

        err = AM_InsertEntry(indexFD, 'i', 4, (char*)&population, rid); // insert returned rid into index with
                                                                        // a pointer to the population value
        // Use the population field as the field to index on

        checkerr(err);
    }
    fclose(fp);
    Table_Close(tbl);
    err = PF_CloseFile(indexFD);
    checkerr(err);
    return sch;
}

int main()
{
    loadCSV();
}
