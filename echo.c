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
		printf("Invalid argument count; need something to echo.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();
	
	printf(argv[1]);
	
	putchar('\n');

	return 0;
}