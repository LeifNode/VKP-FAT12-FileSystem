#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "sectors.h"
#include "fat.h"
#include "sharedmemory.h"
#include "shell.h"
#include "fileio.h"
#include <stdio.h>
#include <ctype.h>

int main(int argc, char* argv[])
{	
	//TODO: relative and absolute file paths
	if (argc != 2)
	{
		printf("Invalid argument count; touch takes the path of the file to remove.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	if (strlen(argv[1]) < 2 && (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0))
	{
		printf("Cannot make file %s.\n", argv[1]);
		exit(1);
	}
	
	char* path = NULL;
	char* fileName = NULL;
	char* extension = NULL;
	
	parsePathFileExtension(argv[1], &path, &fileName, &extension);
	
	/*FILE_HEADER* selectedFile = (FILE_HEADER*)*/
	
	FILE_HEADER *selectedFile = NULL;
	
	findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);

	if (selectedFile != NULL)
	{
		printf("File %s already exists.\n", argv[1]);
		exit(1);
	}
	
	FILE_HEADER_REG* newHeader = getNextFreeDirectoryEntry(getDirStackTop(sharedMem));
	
	if (newHeader == NULL)
	{
		printf("Failed to allocate file header.\n");
		exit(1);
	}
	
	unsigned int newSector = getNextFreeSector();
	
	if (newSector == 0xFFF)
	{
		printf("Failed to allocate file sector.\n");
		collapseDirectory(getDirStackTop(sharedMem));
		exit(1);
	}
	
	unsigned char* fat1 = find_sector(FAT1_OFFSET);
	unsigned char* fat2 = find_sector(FAT2_OFFSET);
	
	set_fat_entry(newSector, 0xFFF, fat1);
	set_fat_entry(newSector, 0xFFF, fat2);
	
	unsigned char* sectorMem = find_sector(DATA_OFFSET + newSector);

	memset(sectorMem, 0, BYTES_PER_SECTOR);
	memset(newHeader, 0, sizeof(FILE_HEADER));
	
	newHeader->first_logical_cluster = newSector;
	
	if (fileName)
	{
		int nameLen = strlen(fileName);
		for (int i = 0; i < 8 && i < nameLen; i++)
		{
			newHeader->file_name[i] = toupper(fileName[i]);
		}
	}
	
	if (extension)
	{
		int entensionLen = strlen(extension);
		for (int i = 0; i < 3 && i < entensionLen; i++)
		{
			newHeader->extension[i] = toupper(extension[i]);
		}
	}
	
	return 0;
}