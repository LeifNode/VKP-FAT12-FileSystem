#ifndef SECTORS_H
#define SECTORS_H

#include "common.h"

int read_sector(int sector_number, unsigned char* buffer);
int write_sector(int sector_number, unsigned char* buffer);

#endif