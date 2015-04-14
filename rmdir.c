#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; rm takes the path of the directory to remove.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	if (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0)
	{
		printf("Cannot use rmdir on %s.\n", argv[1]);
	}
	else
	{
		//FILE_HEADER* selectedFile = (FILE_HEADER*)findFile(argv[1], getDirStackTop(sharedMem));
		
		FILE_HEADER *selectedFile = NULL;
		
		findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);

		if (selectedFile != NULL && 
		    (selectedFile->header.attributes & FILE_ATTR_SUBDIRECTORY) != 0 &&
			selectedFile->header.attributes != 0x0f)//Make sure this file is valid
		{
			//TODO: Support absolute file paths
			
			if (!isDirectoryEmpty(selectedFile))
			{
				printf("Directory still has files.\n");
			}
			else
			{
				freeFatChain(selectedFile->header.first_logical_cluster, true);
				
				memset(selectedFile, 0, sizeof(FILE_HEADER));
				
				collapseDirectory(getDirStackTop(sharedMem));
			}
		}
		else
		{
			printf("Specified file %s is not a directory.\n", argv[1]);
		}
	}

	return 0;
}