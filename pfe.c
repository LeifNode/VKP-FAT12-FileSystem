#include "common.h"
#include "mount.h"
#include "bootsector.h"
#include "fat.h"

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Invalid argument count; pfe takes the path to the floppy image along with the start and end indices of the FAT table.\n");
		exit(1);
	}

	int startIndex = atoi(argv[2]);
	int endIndex = atoi(argv[3]);

	openFileSystem(argv[1]);
	readBootSector();

	pfe(startIndex, endIndex);

	closeFileSystem();

	return 0;
}