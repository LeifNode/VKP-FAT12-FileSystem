#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#endif

#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>

extern uint16_t BYTES_PER_SECTOR;


#endif
