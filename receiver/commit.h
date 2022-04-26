#define checkerr(err)        \
    {                        \
        if (err < 0)         \
        {                    \
            PF_PrintError("err"); \
            exit(1);         \
        }                    \
    }

int commit_insert(Table *tbl, Table_row *tr);       // insert a new row into a table
                                                    // Returns C_OK on success, C_ERROR on failure

int commit_delete(Table *tbl, RecId rid);           // delete a row from a table
                                                    // Returns C_OK on success, C_ERROR on failure