#ifndef PFE_H
#define PFE_H

#include "common.h"

///@brief Prints out a human-readable table of all of the FAT entries in the FAT table.
///@brief param [in]	start	The number of the first FAT entry to start reading from (start with 2 since first 2 are unused).
///@brief param [in]	end		The number of the last FAT entry to read from (must be at least 2 since first 2 are unused).
void pfe(int start, int end);

#endif
