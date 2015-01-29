#ifndef CAT_H
#define CAT_H

#include "fileio.h"

///@brief Given a regular 8.1 file header, prints out the contents of the file to console.
///@param [in] file	A pointer to a FILE_HEADER_REG.
void cat(const FILE_HEADER_REG* file);

#endif