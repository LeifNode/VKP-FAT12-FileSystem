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
	
	if (sharedMem->image_path[0] != '\0')
	{
		printf("File system is already mounted. Use umount first to unmount the file system.\n");
		exit(1);
	}
	
	if (!openFileSystem(argv[1]))
	{
		printf("Could not mount image %s\n", argv[1]);
		exit(1);
	}
	
	int argLen = 0;
	
	for (int i = 0; i < 255; i++)
	{
		if (argv[1][i] == '\0')
		{
			argLen = i + 1;
			break;
		}
	}
	
	memcpy(&sharedMem->image_path, argv[1], argLen);
	sharedMem->image_path[argLen] = '\0';
	//printf("Mount path: %s\n", sharedMem->image_path);
	sharedMem->boot_sector = *getBootSector(FILE_SYSTEM);
	sharedMem->current_dir_flc = 0;
	
	return 0;
}