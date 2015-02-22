#include "common.h"
#include "imageutils.h"
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
	
	if (sharedMem->file_system != NULL)
	{
		printf("File system is already mounted. Use umount first to unmount the file system.\n");
		exit(1);
	}
	
	
}