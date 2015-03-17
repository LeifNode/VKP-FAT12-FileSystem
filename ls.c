#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "fileio.h"

#include "sectors.h"

#include "global_limits.h"

//An array to hold files found.
FILE_HEADER_REG* fileList[MAX_LISTABLE_FILES];

int compareFileHeaderByName(const FILE_HEADER *file1, const FILE_HEADER *file2)
{
	return memcmp(file1->header.file_name, file2->header.file_name, 8);
}

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		printf("Too many arguments!\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();
	
	//Clear list.
	memset(fileList, 0, MAX_LISTABLE_FILES);
	
	char *path = NULL;
	
	if(argc > 1)
		path = argv[1];
	
	FILE_HEADER* thisDir;
	
	size_t fileIndex = 0;
	
	if(!path)
	{
		printf("getting sharedmem %p\n", getDirStackTop(sharedMem));
		thisDir = getDirStackTop(sharedMem);
	}
	else
	{
		thisDir = (FILE_HEADER*)findFile(path, getDirStackTop(sharedMem));
		
		if(!thisDir)
		{
			printf("Could not find path\n");
			exit(1);
		}	
	}
	
	printf("This dir is: %p\n", thisDir);
	
	if((thisDir->header.attributes & FILE_ATTR_SUBDIRECTORY) != 0)
	{	
		FILE_HEADER_REG *currentHeader;
		
		//Check if we are in root.
		if(isRoot(thisDir))
		{
			//Set currentHeader to root;
			currentHeader = &thisDir->header;
			
			for (int i = 0; i < MAX_FILES_IN_ROOT_DIR; i++)
			{
				if (currentHeader->attributes != 0x0f)//Is this a long file name?
				{
					if((currentHeader->attributes & FILE_ATTR_HIDDEN) != 0 && *(unsigned char*)(currentHeader) != FILE_DELETED_BYTE)
					{
						//Add file to the list.
						fileList[fileIndex++] = currentHeader;
					}
				}
			}
		}
		else //Search specified directory
		{
			unsigned char* fat = (unsigned char*)find_sector(FAT1_OFFSET);
			uint16_t currentCluster = thisDir->header.first_logical_cluster;
			
			do
			{
				FILE_HEADER_REG* dataSector = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentCluster);
				currentHeader = dataSector;
				
				for (int i = 0; i < 16; i++)
				{
					if (currentHeader->attributes != 0x0f)//Is this a long file name?
					{
						//if((currentHeader->attributes & FILE_ATTR_HIDDEN) != 0 && *(unsigned char*)(currentHeader) != FILE_DELETED_BYTE)
						//{
							//Add file to the list.
							fileList[fileIndex++] = currentHeader;
						//}
					}
					
					currentHeader++;
				}
				
				currentCluster = get_fat_entry(currentCluster, fat);
			} while (currentCluster < 0xFF7);//Loop through all subdirectory sectors in chain
		}			
	}
	else
	{
		printf("That is not a directory!\n");
		exit(1);
	}
	
	printf("%u files found\n", fileIndex);
	
	//Sort by name.
	qsort(fileList, fileIndex, sizeof(FILE_HEADER), compareFileHeaderByName);
	
	//Now we can display.
	
	//char fileName[9];
	
	//memset(fileName, 0, 9);
	
	for(size_t i = 0; i < fileIndex; ++i)
	{
		for(size_t j = 0; j < 8; j++)
			putchar(fileList[i]->file_name[j]);
		//memcpy(fileName, &fileList[i]->file_name, 8);
		//printf("%s\n", fileName);
	}
	
	return 0;
}