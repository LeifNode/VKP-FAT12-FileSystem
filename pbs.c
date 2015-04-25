#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "sharedmemory.h"

///@brief Main function for pbs command.
///@test If pbs is run with any number of arguments, pbs shall print a readout containing information about the boot sector of the currently mounted disk image.
int pbs_main(int argc, char *argv[])
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

int main(int argc, char* argv[])
{
	return pbs_main(argc, argv);
}