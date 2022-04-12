
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tbl.h"
#include "codec.h"
#include "../pflayer/pf.h"

#define SLOT_COUNT_OFFSET 2
#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(EXIT_FAILURE);}}

int  getLen(int slot, byte *pageBuf) {
    Header *header = (Header *)pageBuf;
    return ((slot == 0) ? PF_PAGE_SIZE : header->slotOffsets[slot-1])-header->slotOffsets[slot];
}
int  getNumSlots(byte *pageBuf) {
    Header *header = (Header *)pageBuf;
    return header->numSlots;
}
void setNumSlots(byte *pageBuf, int nslots) {
    Header *header = (Header *)pageBuf;
    header->numSlots = nslots;
}
int  getNthSlotOffset(int slot, char* pageBuf) {
    Header *header = (Header *)pageBuf;
    return header->slotOffsets[slot];
}

/**
   Opens a paged file, creating one if it doesn't exist, and optionally
   overwriting it.
   Returns 0 on success and a negative error code otherwise.
   If successful, it returns an initialized Table*.
 */
int
Table_Open(char *dbname, Schema *schema, bool overwrite, Table **ptable)
{
    // allocate Table structure  and initialize and return via ptable
    // The Table structure only stores the schema. The current functionality
    // does not really need the schema, because we are only concentrating
    // on record storage. 
    int fd, errVal;

    if (overwrite) {
        // Delete the file if it exists
        if ((fd = PF_OpenFile(dbname)) >= 0) {
            if ((errVal = PF_CloseFile(fd)) != PFE_OK) {
                PF_PrintError("Error closing file");
                return errVal;
            }
            if ((errVal = PF_DestroyFile(dbname)) != PFE_OK) {
                PF_PrintError("Error destroying file");
                return errVal;
            }
        }
    }

    // Open the file (create and open if it doesn't exist)
    if ((fd = PF_OpenFile(dbname)) < 0) {
        if ((errVal = PF_CreateFile(dbname)) != PFE_OK) {
            PF_PrintError("Error creating file");
            return errVal;
        }
        if ((fd = PF_OpenFile(dbname)) < 0) {
            PF_PrintError("Error opening file");
            return fd;
        }
    }

    // Initializing the table struct with default values
    Table *table = (Table *) malloc(sizeof(Table));
    table->schema = schema;
    table->fd = fd;
    table->lastPage = malloc(sizeof(int));
    *table->lastPage = -1;
    table->numPages = 0;
    table->pagebuf = (char **) malloc(sizeof(char *));

    // Counting number of pages and fixing the last page into pagebuf
    int prevPage = -1;
    while (PF_GetNextPage(fd, table->lastPage, table->pagebuf) == PFE_OK) {
        table->numPages++;
        PF_UnfixPage(fd, prevPage, false);
        prevPage = *table->lastPage;
    }

    *ptable = table;

    return 0;
}

void
Table_Close(Table *tbl) {
    // Unfix any dirty pages, close file.
    PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
    if (PF_CloseFile(tbl->fd) != PFE_OK) {
        PF_PrintError("Error closing file (2)");
    }
    free(tbl->lastPage);
    free(tbl->pagebuf);
    free(tbl);
}


int
Table_Insert(Table *tbl, byte *record, int len, RecId *rid) {
    // Allocate a fresh page if len is not enough for remaining space
    // Get the next free slot on page, and copy record in the free
    // space
    // Update slot and free space index information on top of page.

    // Allocate a new page if file is empty
    if (*tbl->lastPage == -1) {
        if (PF_AllocPage(tbl->fd, tbl->lastPage, tbl->pagebuf) != PFE_OK) {
            PF_PrintError("Error allocating page");
            return -1;
        }

        Header *header = (Header *) *tbl->pagebuf;
        header->numSlots = 0;
        header->freeSlotOffset = PF_PAGE_SIZE;
    }
    
    Header *header = (Header *) *tbl->pagebuf;
    int nslots = getNumSlots(*tbl->pagebuf);
    // Calculating free space left in the page (after removing the header)
    int freeSpace = header->freeSlotOffset - sizeof(int) - sizeof(short)*(2+nslots);

    // If free space is not enough for the record, allocate a new page
    if (freeSpace < len) {
        PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
        if (PF_AllocPage(tbl->fd, tbl->lastPage, tbl->pagebuf) != PFE_OK) {
            PF_PrintError("Error allocating page");
            return -1;
        }
        header = (Header *) *tbl->pagebuf;
        nslots = header->numSlots = 0;
        header->freeSlotOffset = PF_PAGE_SIZE;
        freeSpace = header->freeSlotOffset - sizeof(int) - sizeof(short)*2;
    }
    
    assert(freeSpace >= len);     // Assuming that the record fits in the page
    header->freeSlotOffset -= len;          // Shifting the free space offset
    memcpy(*tbl->pagebuf+header->freeSlotOffset, record, len);  // Copying the record into pagebuf
    setNumSlots(*tbl->pagebuf, nslots+1);    // Updating the number of slots
    header->slotOffsets[nslots] = header->freeSlotOffset;      // Updating the slot offsets array
    *rid = (*tbl->lastPage << 16) + nslots;
    return 0;
}

#define checkerr(err) {if (err < 0) {PF_PrintError(); exit(EXIT_FAILURE);}}

/*
  Given an rid, fill in the record (but at most maxlen bytes).
  Returns the number of bytes copied.
 */
int
Table_Get(Table *tbl, RecId rid, byte *record, int maxlen) {
    int slot = rid & 0xFFFF;
    int pageNum = rid >> 16;

    // PF_GetThisPage(pageNum)
    // In the page get the slot offset of the record, and
    // memcpy bytes into the record supplied.
    // Unfix the page
    
    char **pagebuf = (char **) malloc(sizeof(char *));
    bool unfix = false;
    *pagebuf = *tbl->pagebuf;

    // If the page is not in the buffer, fix it (the last page is always in the buffer)
    if (pageNum != *tbl->lastPage) {
        if (PF_GetThisPage(tbl->fd, pageNum, pagebuf) != PFE_OK) {
            PF_PrintError("Error getting page");
            return -1;
        }
        unfix = true;
    }
    
    // Getting the record
    Header *header = (Header *) *pagebuf;
    int slotOffset = getNthSlotOffset(slot, *pagebuf);
    int len = getLen(slot, *pagebuf);
    if (len > maxlen) {
        len = maxlen;
    }
    memcpy(record, *pagebuf+slotOffset, len);

    if (unfix) {
        PF_UnfixPage(tbl->fd, pageNum, false);
    }
    free(pagebuf);
    
    return len; // return size of record
}

void
Table_Scan(Table *tbl, void *callbackObj, ReadFunc callbackfn) {
    // For each page obtained using PF_GetFirstPage and PF_GetNextPage
    //    for each record in that page,
    //          callbackfn(callbackObj, rid, record, recordLen)
    
    int *pagenum = (int *) malloc(sizeof(int));
    *pagenum = -1;
    char **pagebuf = (char **) malloc(sizeof(char *));
    int prevPage = -1;

    // Unfix the last page for PF_GetNextPage to work
    PF_UnfixPage(tbl->fd, *tbl->lastPage, true);
    // Iterating over all pages and unfixing the previous ones
    while (PF_GetNextPage(tbl->fd, pagenum, pagebuf) == PFE_OK) {
        Header *header = (Header *) *pagebuf;
        int nslots = header->numSlots;
        for (int i = 0; i < nslots; i++) {
            int len = getLen(i, *pagebuf);
            int rid = (*pagenum << 16) + getNthSlotOffset(i, *pagebuf);
            callbackfn(callbackObj, rid, *pagebuf+header->slotOffsets[i], len);
        }
        PF_UnfixPage(tbl->fd, prevPage, false);
        prevPage = *pagenum;
    }
    free(pagebuf);
    free(pagenum);
}


