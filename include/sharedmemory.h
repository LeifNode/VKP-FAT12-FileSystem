#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "common.h"
#include "bootsector.h"
#include "fileio.h"

#include "global_limits.h"

///@brief A structure to facilitate shared data between shell and applications.
typedef struct SHELL_SHARED_MEMORY
{
	///@brief A full copy of the boot sector in use.
	BOOT_SECTOR boot_sector;
	///@brief First Logical Cluster of current directory. If in root directory this will be 0
    int current_dir_flc;
	///@brief Offset into file system for file entry that describes this subdirectory.
	void* current_dir_offset;
	//uint8_t* file_system;
	///@brief String holding a path to the current loaded working drive image.
	char image_path[MAX_SHM_PATH_SIZE];
	///@brief String holding a path to the current working directory.
	char working_dir_path[MAX_SHM_PATH_SIZE];
	
	///Directory stack
	///@brief The index of the top entry in the directory stack.
	int stack_top_index;
	///@brief The directory stack used for quick working directory traversal.
	void* directory_stack[MAX_DIR_STACK_ENTRIES];
	
	///@brief The index of the next free fat.
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
///@param[in]	sharedMemory	The SHELL_SHARED_MEMORY object to read from.
///@return	Returns a pointer to the FILE_HEADER.
FILE_HEADER* getDirStackTop(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Gets the address of a FILE_HEADER at the specified index of the stored directory stack.
///@param[in]	sharedMemory	The SHELL_SHARED_MEMORY object to read from.
///@param[in]	index			The index to read from.
///@return	Returns a pointer to the FILE_HEADER.
FILE_HEADER* getDirStackIndex(SHELL_SHARED_MEMORY* sharedMemory, int index);

///@brief	Pops the directory stack and returns a pointer to the topmost FILE_HEADER popped.
///@param[in]	sharedMemory	The SHELL_SHARED_MEMORY object to operate on.
///@return	Returns a pointer to the FILE_HEADER popped.
FILE_HEADER* popDirStack(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Pushes a pointer to a FILE_HEADER the directory stack.
///@param[in]	sharedMemory	The SHELL_SHARED_MEMORY object to operate on.
///@param[in]	header			The FILE_HEADER pointer to be pushed.
void pushDirStack(SHELL_SHARED_MEMORY* sharedMemory, FILE_HEADER* header);

///@brief	Prints the working directory.
///@param[in] sharedMemory	The SHELL_SHARED_MEMORY object to read from.
void printWorkingDirectory(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Prints the working directory path.
///@param[in] sharedMemory	The SHELL_SHARED_MEMORY object to read from.
void printWorkingDirectoryPath(SHELL_SHARED_MEMORY* sharedMemory);

///@brief	Returns a working path as a string, given a pointer to a SHELL_SHARED_MEMORY object containing a directory stack.
///@param[in]	sharedMemory	The SHELL_SHARED_MEMORY object to read from.
///@return	Returns a const char string containing the path.
///@warning	The pointer returned is to a statically allocated buffer within the function and should NOT be freed via free()!  A copy should be made (e.g. via strdup()) if any manipulation is to be done.
const char* getWorkingPathFromStack(SHELL_SHARED_MEMORY* sharedMemory);

#endif
