#ifndef FAT_H
#define FAT_H

#include "common.h"

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat);
void set_fat_entry(int fat_entry_number, int value, unsigned char* fat);

///@brief Gets the number of free sectors on disk.
///@return	Returns a uint16_t.
uint16_t get_free_sector_count();

///@brief Prints out a human-readable table of all of the FAT entries in the FAT table.
///@brief param[in]	start	The number of the first FAT entry to start reading from (start with 2 since first 2 are unused).
///@brief param[in]	end		The number of the last FAT entry to read from (must be at least 2 since first 2 are unused).
void pfe(int start, int end);

///@brief Frees a FAT chain.
///@param[in]	fatStart	An index of the fat entry to start at.
///@param[in]	zeroMemory	A boolean value indicating whether or not to zero the freed memory.
void freeFatChain(int fatStart, bool zeroMemory);

///@brief Returns the number of the next free sector.
///@return Returns the number of the next free sector as an unsigned int.
unsigned int getNextFreeSector();

///@brief Links a sector onto the specified sector and updates the FAT tables to extend the FAT entry chain.
///@param[in] startSector The sector number to append to.
///@return Returns the sector that was allocated and appended to the end.
unsigned int appendSector(int startSector);

#endif
