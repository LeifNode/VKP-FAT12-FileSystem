#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"

///@brief Main function for pfe command.
///@test If pfe is provided with exactly two arguments indicating the start and end entry indices within the FAT table for which to print a range of FAT entries.
///@test If pfe is provided with any number of arguments other than two, pfe shall exit printing, "Invalid argument count; pfe takes the start and end indices of the FAT table indicating a range of FAT entries to print out.".
///@test If any of pfe's two arguments are not a valid number, that argument shall be interpreted as 0.
int pfe_main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Invalid argument count; pfe takes the start and end indices of the FAT table indicating a range of FAT entries to print out.\n");
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

int main(int argc, char* argv[])
{
	return pfe_main(argc, argv);
}