#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"


///@brief Main function for rm command.
///@test If rm is given a single argument containing a valid path to a file, rm shall delete that file from the image.
///@test If rm is given any number of arguments other than one, rm shall exit printing, "Invalid argument count; rm takes the path of the file to remove.".
///@test If rm is used successfully, rm shall attempt to collapse the directory it is deleting from.
///@test If rm is given a single argument containing a valid path to a file, however, that file is a subdirectory or long file entry, then rm shall exit printing "Could not rm file [path].".
int rm_main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; rm takes the path of the file to remove.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	if (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0)
	{
		printf("Cannot use rm on %s.\n", argv[1]);
	}
	else
	{
		//FILE_HEADER* selectedFile = (FILE_HEADER*)findFile(argv[1], getDirStackTop(sharedMem));
		
		FILE_HEADER *selectedFile = NULL;
		
		findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);

		if (selectedFile != NULL && 
		    (selectedFile->header.attributes & FILE_ATTR_SUBDIRECTORY) == 0 &&
			selectedFile->header.attributes != 0x0f)//Make sure this file is valid
		{
			//TODO: Support absolute file paths
			deleteFile(selectedFile);
			collapseDirectory(getDirStackTop(sharedMem));
		}
		else
		{
			printf("Could not rm file %s.\n", argv[1]);
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	return rm_main(argc, argv);
}