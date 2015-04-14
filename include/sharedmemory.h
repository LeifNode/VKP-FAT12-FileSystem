#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "common.h"
#include "bootsector.h"
#include "fileio.h"

///@brief A structure to facilitate shared data between shell and applications.
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
	
	int next_free_fat;
} SHELL_SHARED_MEMORY;

//#define SHMKEY 7435
#define SHMKEY 2467
//#define SHMNAME "/vkpshare"
#define SHMNAME "/vkpmemspace"


///@brief	Allocates an internal shared memory file buffer.
///@return	N/A (call mapShared() after this to get a memory-mapped pointer to what this allocates)
void createShared();

///@brief	Gets a memory-mapped pointer to shared memory allocated by a call to createShared().
///@return	Returns a pointer to a SHELL_SHARED_MEMORY struct.
SHELL_SHARED_MEMORY* mapShared();

///@brief	Gets the pointer to shared memory last set up by a call to mapShared().
///@return	Returns a pointer to a SHELL_SHARED_MEMORY struct.
SHELL_SHARED_MEMORY* getSharedMemoryPtr();

///@brief	Called to unmap the pointer to shared memory
void unmapShared();


FILE_HEADER* getDirStackTop(SHELL_SHARED_MEMORY* sharedMemory);
FILE_HEADER* getDirStackIndex(SHELL_SHARED_MEMORY* sharedMemory, int index);
FILE_HEADER* popDirStack(SHELL_SHARED_MEMORY* sharedMemory);
void pushDirStack(SHELL_SHARED_MEMORY* sharedMemory, FILE_HEADER* header);

void printWorkingDirectory(SHELL_SHARED_MEMORY* sharedMemory);
void printWorkingDirectoryPath(SHELL_SHARED_MEMORY* sharedMemory);

const char* getWorkingPathFromStack(SHELL_SHARED_MEMORY* sharedMemory);

#endif
