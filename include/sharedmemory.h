#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "common.h"
#include "bootsector.h"
#include "fileio.h"

typedef struct SHELL_SHARED_MEMORY
{
	BOOT_SECTOR boot_sector;
    int current_dir_flc;//First Logical Cluster of current directory. If in root directory this will be 0
	void* current_dir_offset;//Offset into file system for file entry that describes this subdirectory
	//uint8_t* file_system;
	char image_path[256];
	char working_dir_path[256];
	
	//Directory stack
	int stack_top_index;
	void* directory_stack[64];
} SHELL_SHARED_MEMORY;

#define SHMKEY 7435
#define SHMNAME "/vkpshare"

void createShared();
SHELL_SHARED_MEMORY* mapShared();
void unmapShared();

FILE_HEADER* getDirStackTop(SHELL_SHARED_MEMORY* sharedMemory);
FILE_HEADER* popDirStack(SHELL_SHARED_MEMORY* sharedMemory);
void pushDirStack(SHELL_SHARED_MEMORY* sharedMemory, FILE_HEADER* header);

#endif
