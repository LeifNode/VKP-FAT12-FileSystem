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
	
	if (strcmp(argv[1], ".") == 0)
	{}
	else if (strcmp(argv[1], "..") == 0)
	{
		popDirStack(sharedMem);
		
		FILE_HEADER* top = getDirStackTop(sharedMem);
		
		if (top != NULL)
		{
			printFileHeader(top);
		}
		else
		{
			printf("root\n");
		}
	}
	else
	{
		FILE_HEADER* selectedDir = (FILE_HEADER*)findFile(argv[1], getDirStackTop(sharedMem));

		if (selectedDir != NULL && (selectedDir->header.attributes & FILE_ATTR_SUBDIRECTORY) != 0)//Check if this is actually a directory
		{
			printFileHeader(selectedDir);
			
			pushDirStack(sharedMem, selectedDir);
		}
	}

	return 0;
}