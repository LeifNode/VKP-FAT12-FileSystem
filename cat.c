#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "fileio.h"

///@brief Main function for cat command.
///@test If given the name of a file path culminating in a file name with an extension, cat shall display file contents.
///@test If given '.' or '..', cat shall print "Cannot use cat on ./...".
///@test If number of arguments is greater or less than one, cat shall print "Invalid argument count; cat takes a file name to cat.".
///@test If path given points to root, attributes indicate a long filename (should never occur), or a file is not a subdirectory, cat shall print "File %s is incompatible with cat.".
///@test If path given does not lead to a file at all, cat shall print "Could not find file [filename] to cat!".
int cat_main(int argc, char* argv[])
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

		if(selectedFile != NULL)
		{
			if ((selectedFile->header.attributes & FILE_ATTR_SUBDIRECTORY) == 0 &&
				selectedFile->header.attributes != 0x0f && 
				selectedFile->header.first_logical_cluster != 0 /*&&
				selectedFile->header.file_size > 0*/)//Make sure this file is valid
			{
				cat(&selectedFile->header);
				
				printf("\n");
			}
			else
			{
				printf("File %s is incompatible with cat.\n", getFileNameStringFromFileHeader(selectedFile));
			}
		}
		else
			printf("Could not find file %s to cat!\n", argv[1]);
		
	}

	return 0;
}

int main(int argc, char* argv[])
{
	return cat_main(argc, argv);
}