#include "common.h"
#include "mount.h"
#include "bootsector.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count. pbs takes the path of the floppy to mount.");
		exit(1);
	}

	openFileSystem(argv[1]);

	readBootSector();
	printBootSector();

	closeFileSystem();

	return 0;
}