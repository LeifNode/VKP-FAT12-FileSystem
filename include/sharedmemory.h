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

///@brief	Gets the address of FILE_HEADER at the top of the stored directory stack.
///@param [in]	sharedMemory	The SHELL_SHARED_MEMORY object to read from.
///@return	Returns a pointer to the FILE_HEADER.
FILE_HEADER* getDirStackTop(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Gets the address of a FILE_HEADER at the specified index of the stored directory stack.
///@param [in]	sharedMemory	The SHELL_SHARED_MEMORY object to read from.
///@param [in]	index			The index to read from.
///@return	Returns a pointer to the FILE_HEADER.
FILE_HEADER* getDirStackIndex(SHELL_SHARED_MEMORY* sharedMemory, int index);

///@todo Is this correct?
///@brief	Pops the directory stack and returns a pointer to the topmost FILE_HEADER popped.
///@param [in]	sharedMemory	The SHELL_SHARED_MEMORY object to operate on.
///@return	Returns a pointer to the FILE_HEADER popped.
FILE_HEADER* popDirStack(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Pushes a pointer to a FILE_HEADER the directory stack.
///@param [in]	sharedMemory	The SHELL_SHARED_MEMORY object to operate on.
///@param [in]	header			The FILE_HEADER pointer to be pushed.
void pushDirStack(SHELL_SHARED_MEMORY* sharedMemory, FILE_HEADER* header);

///@todo Define and tell difference between printWorkingDirectoryPath().
void printWorkingDirectory(SHELL_SHARED_MEMORY* sharedMemory);
///@todo Define and tell difference between printWorkingDirectory().
void printWorkingDirectoryPath(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Returns a working path as a string, given a pointer to a SHELL_SHARED_MEMORY object containing a directory stack.
///@param [in]	sharedMemory	The SHELL_SHARED_MEMORY object to read from.
///@return	Returns a const char string containing the path.  THIS IS STATICALLY ALLOCATED AND SHOULD NOT BE FREED VIA free()!
const char* getWorkingPathFromStack(SHELL_SHARED_MEMORY* sharedMemory);

#endif
