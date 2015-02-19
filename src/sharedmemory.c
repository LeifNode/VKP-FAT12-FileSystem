#include "sharedmemory.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

SHELL_SHARED_MEMORY* mapShared()
{
	SHELL_SHARED_MEMORY* sharedMem;
	int shmid;

	if ((shmid = shmget(SHMKEY, sizeof(SHELL_SHARED_MEMORY), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
    }
	
    if ((sharedMem = shmat(shmid, NULL, 0)) == (SHELL_SHARED_MEMORY *) -1) {
        perror("shmat");
    }
	
	return sharedMem;
}