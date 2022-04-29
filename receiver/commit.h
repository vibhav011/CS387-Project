#ifndef _COMMIT_H_
#define _COMMIT_H_
#include "../utils.h"
#include "../dblayer/tbl.h"
#include <vector>

int commit_insert(Table *tbl, Table_Row *tr, RecId *rid);       // insert a new row into a table
                                                    // Returns C_OK on success, C_ERROR on failure

int commit_delete(Table *tbl, RecId rid);           // delete a row from a table
                                                    // Returns C_OK on success, C_ERROR on failure

int execute_commit(vector<int>* ChangeIndices);     // commit all the changes in the change logs
                                                    // Returns C_OK on success, C_ERROR on failure

int execute_rollback_single(Table *tbl, ChangeLog& change_log, MappingLog& mapping_log);

int execute_rollback(vector<int>* ChangeIndices);
#endif