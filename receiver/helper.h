#ifndef _HELPER_H_
#define _HELPER_H_
#include <string>
#include "query.h"
#include "../utils.h"

int dump_log(ChangeLog& change_log, string filename);       // Returns C_OK on success, C_ERROR on failure
int read_log(ChangeLog& change_log, string filename);       // Returns C_OK on success, C_ERROR on failure

#endif