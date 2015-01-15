#ifndef FAT_H
#define FAT_H

#include "common.h"

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat);
void set_fat_entry(int fat_entry_number, int value, unsigned char* fat);

#endif
