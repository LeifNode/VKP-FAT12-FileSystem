#include "common.h"
#include "mount.h"
#include "bootsector.h"
#include "sharedmemory.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count. pbs takes the path of the floppy to mount.\n");
		exit(1);
	}

	SHELL_SHARED_MEMORY* sharedMem = mapShared();
	
	if (!openFileSystem(argv[1]))
	{
	    printf("Failed to mount %s\n", argv[1]);
	    return -1;
	}

	readBootSector();
	printBootSector();

	closeFileSystem();

	return 0;
}
