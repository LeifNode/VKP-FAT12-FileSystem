#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "sharedmemory.h"

int main(int argc, char *argv[])
{
	/* if (argc != 2)
	{
		printf("Invalid argument count. pbs takes the path of the floppy to mount.\n");
		exit(1);
	} */

	SHELL_SHARED_MEMORY* sharedMem = mapShared();
	
	/* if (!openFileSystem(argv[1]))
	{
	    printf("Failed to mount %s\n", argv[1]);
	    return -1;
	} */

	/* openFileSystem(sharedMem->image_path);
	
	BOOT_SECTOR* bootSector = getBootSector(FILE_SYSTEM);
	printf("Printing boot sector\n"); */
	printBootSector(&sharedMem->boot_sector);

	//closeFileSystem();

	return 0;
}
