#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "common.h"
#include "bootsector.h"

typedef struct SHELL_SHARED_MEMORY
{
    int current_dir_flc;//First Logical Cluster of current directory. If in root directory this will be 0
    
} SHELL_SHARED_MEMORY;

#define SHMKEY 7433

SHELL_SHARED_MEMORY* mapShared();

#endif
