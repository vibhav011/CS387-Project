#ifndef _COMMIT_H_
#define _COMMIT_H_
#include "utils.h"

int commit_insert(Table *tbl, Table_Row *tr);       // insert a new row into a table
                                                    // Returns C_OK on success, C_ERROR on failure

int commit_delete(Table *tbl, RecId rid);           // delete a row from a table
                                                    // Returns C_OK on success, C_ERROR on failure

int execute_commit(vector<int>* ChangeIndices);     // commit all the changes in the change logs
                                                    // Returns C_OK on success, C_ERROR on failure
#endif