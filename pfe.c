#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Invalid argument count; pfe takes the path to the floppy image along with the start and end indices of the FAT table.\n");
		exit(1);
	}

	int startIndex = atoi(argv[1]);
	int endIndex = atoi(argv[2]);
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	pfe(startIndex, endIndex);

	return 0;
}