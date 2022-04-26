#include "query.h"
#include "commit.h"
#include "string.h"
#include "../dblayer/tbl.h"
#include "../pflayer/pf.h"
#include "../dblayer/codec.h"

#define MAX_PAGE_SIZE 4000

int commit_insert(Table *tbl, Table_row *tr){
    int numfields = tbl->schema->numColumns;
    char record[MAX_PAGE_SIZE];

    int pos = 0, bytes_added = 0;
    for(int i=0; i<tbl->schema->numColumns; i++){
        switch (tbl->schema->columns[i]->type)
        {
        case VARCHAR:
            int len = (*tr->fields[i].str_val).length();
            if(pos + len + 2 > MAX_PAGE_SIZE){
                printf("insert error: no space left for filling fields in record\n");
                return -1;
            }
            char cpy[len+1];
            strcpy(cpy, (*tr->fields[i].str_val).c_str());
            bytes_added = EncodeCString(cpy, record+pos, 257);
            pos += bytes_added;
            break;

        case INT:
            if(pos+4>MAX_PAGE_SIZE){
                printf("insert error: no space left for int\n");
                return -1;
            }   
            bytes_added = EncodeInt(tr->fields[i].int_val, record+pos);
            pos += bytes_added;
            break;
        
        case DOUBLE:
            if(pos+8>MAX_PAGE_SIZE){
                printf("insert error: no space left for double/float value");
            }
            bytes_added = EncodeDouble(tr->fields[i].float_val, record+pos);
            pos += bytes_added;
            break;

        default:
            printf("insert error: we should not reach here\n");
            break;
        }
    }

    RecId rid;
    int err = Table_Insert(tbl, record, pos, &rid);
    checkerr(err);
}