#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "fileio.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; cd takes a file name to search for.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();
	
	FILE_HEADER* subdir = (FILE_HEADER*)findFile("SUBDIR", NULL);
	
	FILE_HEADER* example = (FILE_HEADER*)findFile("EXAMPLE", subdir);

	printFileHeader(example);

	return 0;
}