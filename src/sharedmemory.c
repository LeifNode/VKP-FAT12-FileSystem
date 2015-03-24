#include "sharedmemory.h"
// #include <sys/types.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>

// SHELL_SHARED_MEMORY* mapShared()
// {
	// SHELL_SHARED_MEMORY* sharedMem;
	// int shmid;
	
	// if ((shmid = shmget(SHMKEY, 16, IPC_CREAT | 0666)) < 0) {
        // perror("shmget");
    // }
	
    // if ((sharedMem = shmat(shmid, NULL, 0)) == (SHELL_SHARED_MEMORY *) -1) {
        // perror("shmat");
    // }
	
	// return sharedMem;
// }

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "imageutils.h"

void createShared()
{
	int fd;
	
	fd = shm_open(SHMNAME, O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	ftruncate(fd, sizeof(SHELL_SHARED_MEMORY));
	
	close(fd);
}

SHELL_SHARED_MEMORY* mapShared()
{
	SHELL_SHARED_MEMORY* sharedMem = NULL;
	int fd = -1;
	
	fd = shm_open(SHMNAME, O_RDWR, 0);
	
	sharedMem = (SHELL_SHARED_MEMORY*) mmap(NULL, sizeof(SHELL_SHARED_MEMORY), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	close(fd);
	
	return sharedMem;
}

void unmapShared()
{
	shm_unlink(SHMNAME);
}

FILE_HEADER* getDirStackTop(SHELL_SHARED_MEMORY* sharedMemory)
{
	void* offsetPtr = sharedMemory->directory_stack[sharedMemory->stack_top_index];
	
	if (offsetPtr != NULL)
		return (FILE_HEADER*)((uint64_t)offsetPtr + (uint64_t)FILE_SYSTEM);
	else
		return NULL;
}

FILE_HEADER* getDirStackIndex(SHELL_SHARED_MEMORY* sharedMemory, int index)
{
	void* offsetPtr = sharedMemory->directory_stack[index];
	
	if (index <= sharedMemory->stack_top_index && offsetPtr != NULL)
		return (FILE_HEADER*)((uint64_t)offsetPtr + (uint64_t)FILE_SYSTEM);
	else
		return NULL;
}

FILE_HEADER* popDirStack(SHELL_SHARED_MEMORY* sharedMemory)
{
	FILE_HEADER* top = getDirStackTop(sharedMemory);
	
	sharedMemory->stack_top_index--;
	if (sharedMemory->stack_top_index < 0)
	{
		//printf("Stack pop out of bounds.\n");
		sharedMemory->stack_top_index = 0;
		
		return NULL;
	}
	
	FILE_HEADER* newTop = getDirStackTop(sharedMemory);
	if (newTop != NULL)
	{
		sharedMemory->current_dir_flc = newTop->header.first_logical_cluster;
		sharedMemory->current_dir_offset = (void*)((uint64_t)newTop - (uint64_t)FILE_SYSTEM);
	}
	else
	{
		sharedMemory->current_dir_flc = 0;
		sharedMemory->current_dir_offset = NULL;
	}
	
	return top;
}

void pushDirStack(SHELL_SHARED_MEMORY* sharedMemory, FILE_HEADER* header)
{
	sharedMemory->stack_top_index++;
	sharedMemory->current_dir_flc = header->header.first_logical_cluster;
	if (sharedMemory->stack_top_index > 63)
	{
		printf("Stack push out of bounds.\n");
		sharedMemory->stack_top_index = 63;
		return;
	}
	
	sharedMemory->current_dir_offset = (void*)((uint64_t)header - (uint64_t)FILE_SYSTEM);
	sharedMemory->directory_stack[sharedMemory->stack_top_index] = sharedMemory->current_dir_offset;
}

void printWorkingDirectory(SHELL_SHARED_MEMORY* sharedMemory)
{
	if (sharedMemory->stack_top_index == 0 || getDirStackTop(sharedMemory) == NULL)
		printf("/");
	else
	{
		FILE_HEADER* header = getDirStackTop(sharedMemory);
		for (int i = 0; i < 8; i++)
		{
			if (header->header.file_name[i] != ' ')
				printf("%c", header->header.file_name[i]);
			else 
				break;
		}
	}
}
void printWorkingDirectoryPath(SHELL_SHARED_MEMORY* sharedMemory)
{
	
}