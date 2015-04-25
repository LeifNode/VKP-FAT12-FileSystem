#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "sharedmemory.h"

///@brief Main function for mount command.
///@test If mount is provided with a single argument describing a valid OS path to a FAT12-formatted bootable floppy image, mount shall load the image and make it available for use as a file system.
///@test If mount is provided with a single argument describing an invalid OS path, mount shall exit printing, "Could not mount image [image_path]".
///@test If mount is provided with a number of arguments other than one, mount shall exit printing, "Invalid argument count. mount takes the path of the floppy to mount.".
///@test if mount has successfully mounted an image in the past during execution, mount shall exit printing, "File system at [mounted_image_path] is already mounted.".
int mount_main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count. mount takes the path of the floppy to mount.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();
	
	if (sharedMem->image_path[0] != '\0')
	{
		printf("File system at %s is already mounted.\n", sharedMem->image_path);
		//exit(1);
	}
	
	memset(sharedMem, 0, sizeof(sharedMem));
	
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

int main(int argc, char* argv[])
{
	return mount_main(argc, argv);
}