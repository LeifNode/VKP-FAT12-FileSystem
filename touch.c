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

///@brief Main function for touch.
///@test If provided with a single argument containing a non-existent filename, touch shall create a file with the given name within the current working directory.
///@test If provided with a single argument containing a path and culminating in a non-existent filename, touch shall create a file with the given filename within the provided directory.
///@test If provided with anything other than one argument, touch shall exit printing, "Invalid argument count; touch takes the path of the file to create.".
///@test If provided with a single argument containing a valid path to an existing file or directory, touch shall print "File [file_name] touched." and shall update the timestamp of the file or directory.
///@test If provided with ".", "..", touch shall exit printing, "[entry] is not allowed.".

///@test If there is not enough room in a directory to add a new file, a successful touch call shall expand the directory before attempting to create the new file.

///@test If during the process of trying to create a new file, touch cannot allocate a file header, touch shall exit printing, "Failed to allocate file header.".
///@test If during the process of trying to create a new file, mkdir cannot allocate a file sector, touch shall exit printing, "Failed to allocate file sector.".

///@test If successful in creating a file, touch shall add a timestamp accurate to the closest two seconds to the newly created file.
int touch_main(int argc, char* argv[])
{	
	//TODO: relative and absolute file paths
	if (argc != 2)
	{
		printf("Invalid argument count; touch takes the path of the file to create.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	if (strlen(argv[1]) < 2 && (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0))
	{
		printf("%s is not allowed.\n", argv[1]);
		exit(1);
	}
	
	char* fileNameAndExtension = NULL;
	char* extension = NULL;
	
	char *path = strdup(argv[1]);
	
	//parsePathFileExtension(in, &path, &fileNameAndExtension, &extension);
	
	//Get filename with extension. (Find last slash and everything after it)
	fileNameAndExtension = strrchr(argv[1], '/') + 1;
	
	//End string at path;
	char *lastSlash = strrchr(path, '/');
	
	if(lastSlash)
		*lastSlash = NULL;
	
	/*FILE_HEADER* selectedFile = (FILE_HEADER*)*/
	
	FILE_HEADER_REG *selectedFile = NULL;
	
	findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);

	if (selectedFile != NULL)
	{
		printf("File %s touched.\n", getFileNameStringFromFileHeader(selectedFile));
		
		//Update timestamp.
		createFileDateTime(time(NULL), &selectedFile->creation_date, &selectedFile->creation_time);
	
		selectedFile->last_write_time = selectedFile->creation_time;
		selectedFile->last_write_date = selectedFile->creation_date;
		
		selectedFile->last_access_date = selectedFile->creation_date;
		
		free(path);
		exit(1);
	}
	
	//printf("%s\n%s\n%s\n", path, fileNameAndExtension, extension);
	
	
	FILE_HEADER *selectedDir = NULL;
	
	if(lastSlash)
	{	
		if(!findFile(path, getDirStackTop(sharedMem), &selectedDir))
		{
			printf("Failed to find %s!\n", path);
			free(path);
			exit(1);
		}
	}
	else
	{
		fileNameAndExtension = path;
		selectedDir = getDirStackTop(sharedMem);
	}
	
	FILE_HEADER_REG* newHeader = getNextFreeDirectoryEntry(selectedDir);
	
	if (newHeader == NULL)
	{
		printf("Failed to allocate file header.\n");
		free(path);
		exit(1);
	}
	
	unsigned int newSector = getNextFreeSector();
	
	if (newSector == 0xFFF)
	{
		printf("Failed to allocate file sector.\n");
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
	
	//Copy filename and extension
	if(fileNameAndExtension)
	{
		memcpy(newHeader->file_name, getFileHeaderNameChunkFromFileNameString(fileNameAndExtension), 11);
	}
	
	/*if (fileNameAndExtension)
	{
		int nameLen = strlen(fileNameAndExtension);
		for (int i = 0; i < 8 && i < nameLen; i++)
		{
			newHeader->file_name[i] = toupper(fileNameAndExtension[i]);
		}
	}
	
	if (extension)
	{
		int entensionLen = strlen(extension);
		for (int i = 0; i < 3 && i < entensionLen; i++)
		{
			newHeader->extension[i] = toupper(extension[i]);
		}
	}*/
	
	//Create date- and time-stamp.
	createFileDateTime(time(NULL), &newHeader->creation_date, &newHeader->creation_time);
	
	newHeader->last_write_time = newHeader->creation_time;
	newHeader->last_write_date = newHeader->creation_date;
	
	newHeader->last_access_date = newHeader->creation_date;
	
	newHeader->attributes = 0x20;
	
	
	free(path);
	
	return 0;
}

int main(int argc, char* argv[])
{
	return touch_main(argc, argv);
}