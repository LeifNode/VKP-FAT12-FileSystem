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

void createShared()
{
	int fd;
	
	fd = shm_open(SHMNAME, O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
	ftruncate(fd, sizeof(SHELL_SHARED_MEMORY));
	
	close(fd);
}

SHELL_SHARED_MEMORY* mapShared()
{
	SHELL_SHARED_MEMORY* sharedMem;
	int fd;
	
	fd = shm_open(SHMNAME, O_RDWR, 0);
	
	sharedMem = (SHELL_SHARED_MEMORY*) mmap(NULL, sizeof(SHELL_SHARED_MEMORY), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	close(fd);
	
	return sharedMem;
}

void unmapShared()
{
	shm_unlink(SHMNAME);
}