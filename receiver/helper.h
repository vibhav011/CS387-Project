#ifndef _HELPER_H_
#define _HELPER_H_
#include <string>
#include "query.h"
#include "../utils.h"

int dump_clog(Table *tbl, ChangeLog& change_log, string filename);       // Returns C_OK on success, C_ERROR on failure
int dump_mlog(Table *tbl, MappingLog& mapping_log, string filename);       // Returns C_OK on success, C_ERROR on failure
int read_clog(ChangeLog& change_log, string filename);       // Returns C_OK on success, C_ERROR on failure
int read_mlog(MappingLog& mapping_log, string filename);       // Returns C_OK on success, C_ERROR on failure

#endif