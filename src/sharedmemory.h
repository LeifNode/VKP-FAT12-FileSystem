#include "common.h"
#include "bootsector.h"

#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

typedef ShellSharedMemory
{
    int currentDirFLC;
    
} ShellSharedMem;

#define SHMKEY 7433



#endif
