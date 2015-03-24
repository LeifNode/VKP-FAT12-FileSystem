#ifndef SECTORS_H
#define SECTORS_H

#include "common.h"

///@bug DEPRECATED - use find_sector() instead!
///@brief Reads the contents of a sector given a sector number and places the contents in a user-allocated buffer.
///@param [in]	sector_number	An int describing the number of the sector to read.
///@param [in]	buffer			An unsigned char pointer to a buffer to read the file sector into (allocated by user).
int read_sector(int sector_number, unsigned char* buffer);

///@bug DEPRECATED - use find_sector() instead!
///@brief Writes the contents of a sector provided by the user with a sector number to which to write.
///@param [in]	sector_number	An int describing the number of the sector to write to.
///@param [in]	buffer			A buffer provided by the user containing the sector bytes.
int write_sector(int sector_number, unsigned char* buffer);

///@brief Returns a pointer to a sector in the filesystem memory map given a sector number.
///@param [in]	sector_number	A uint32_t describing the sector number to be found.
///@return	A void pointer pointing to the sector with the given number.
void* find_sector(uint32_t sector_number);

#define BOOT_OFFSET 0
#define FAT1_OFFSET 1
#define FAT2_OFFSET 10
#define ROOT_OFFSET 19
#define DATA_OFFSET 31

#endif
