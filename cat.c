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
		printf("Invalid argument count; cat takes a file name to cat.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();
	
	FILE_HEADER *selectedFile = NULL;
	
	if (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0)
	{
		printf("Cannot use cat on %s.\n", argv[1]);
	}
	else
	{
		findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);

		if (selectedFile != NULL && 
		    (selectedFile->header.attributes & FILE_ATTR_SUBDIRECTORY) == 0 &&
			selectedFile->header.attributes != 0x0f && 
			selectedFile->header.first_logical_cluster != 0 &&
			selectedFile->header.file_size > 0)//Make sure this file is valid
		{
			cat(&selectedFile->header);
			
			printf("\n");
		}
		else
		{
			printf("Could not cat file %s.\n", argv[1]);
		}
	}

	return 0;
}