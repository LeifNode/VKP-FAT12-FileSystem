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
		printf("Cannot make directory %s.\n", argv[1]);
		exit(1);
	}

	char *path = strdup(argv[1]);
	
	char *name = strrchr(path,'/');
	
	if(name)
	{
		//Make path end before the folder name.
		*name = '\0';
		
		//Move up a character.
		name++;
	}
	else
	{
		name = path;
	}

	//parsePathFileExtension(argv[1], &path, &fileName, &extension);

	//FILE_HEADER* selectedFile = (FILE_HEADER*)findFile(argv[1], getDirStackTop(sharedMem));

	FILE_HEADER *selectedFile = NULL;

	findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);


	if (selectedFile != NULL)
	{
		printf("File %s already exists.\n", argv[1]);
		free(path);
		exit(1);
	}
	
	FILE_HEADER *selectedDir = NULL;
	
	//printf("path: %s\n", path);
	
	
	//Check if a path was entered.
	if(path != name)
	{
		if(!findFile(path, getDirStackTop(sharedMem), &selectedDir))
		{
			//This will likely never happen.
			printf("Failed to find %s!\n", path);
			free(path);
			exit(1);
		}
	}
	else
		selectedDir = getDirStackTop(sharedMem);

	FILE_HEADER_REG* newHeader = getNextFreeDirectoryEntry(selectedDir);

	if (newHeader == NULL)
	{
		printf("Failed to allocate directory header.\n");
		free(path);
		exit(1);
	}

	unsigned int newSector = getNextFreeSector();

	if (newSector == 0xFFF)
	{
		printf("Failed to allocate directory sector.\n");
		collapseDirectory(selectedDir);
		free(path);
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
	newHeader->attributes = FILE_ATTR_SUBDIRECTORY;


	/*if (name)
	{
		int nameLen = strlen(name);
		int i = 0;
		for (; i < 8 && i < nameLen; i++)
		{
			newHeader->file_name[i] = toupper(fileName[i]);
		}
		
		for(; i < 8; i++)
		{
			newHeader->file_name[i] = ' ';
		}
	}*/
	
	if(name)
	{
		memcpy(newHeader->file_name, getFileHeaderNameChunkFromFileNameString(name), 11);
	}

	FILE_HEADER_REG* dotDotFile = (FILE_HEADER_REG*)sectorMem;
	FILE_HEADER_REG* dotFile = dotDotFile + 1;

	memset(dotFile->file_name, ' ', 11);
	memset(dotDotFile->file_name, ' ', 11);
	
	dotFile->file_name[0] = '.';
	dotFile->first_logical_cluster = newSector;
	
	dotFile->attributes = 0x10;

	dotDotFile->file_name[0] = '.';
	dotDotFile->file_name[1] = '.';
	
	if(selectedDir)
		dotDotFile->first_logical_cluster = selectedDir->header.first_logical_cluster;
	else
		dotDotFile->first_logical_cluster = 0x00;
	
	dotDotFile->attributes = 0x10;

	//Create date- and time-stamp.
	createFileDateTime(time(NULL), &newHeader->creation_date, &newHeader->creation_time);

	newHeader->last_write_time = newHeader->creation_time;
	newHeader->last_write_date = newHeader->creation_date;
	newHeader->last_access_date = newHeader->creation_date;
	
	dotFile->last_write_time = newHeader->creation_time;
	dotFile->last_write_date = newHeader->creation_date;
	dotFile->last_access_date = newHeader->creation_date;
	
	dotDotFile->last_write_time = newHeader->creation_time;
	dotDotFile->last_write_date = newHeader->creation_date;
	dotDotFile->last_access_date = newHeader->creation_date;
	
	free(path);
	return 0;
}
