#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "common.h"
#include "bootsector.h"

typedef struct SHELL_SHARED_MEMORY
{
	BOOT_SECTOR boot_sector;
    int current_dir_flc;//First Logical Cluster of current directory. If in root directory this will be 0
	void* current_dir_offset;//Offset into file system for file entry that describes this subdirectory
	//uint8_t* file_system;
	char image_path[256];
	char working_dir_path[256];
} SHELL_SHARED_MEMORY;

#define SHMKEY 7435
#define SHMNAME "/vkpshare"

void createShared();
SHELL_SHARED_MEMORY* mapShared();
void unmapShared();

#endif
