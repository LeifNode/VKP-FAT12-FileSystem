#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "fileio.h"

#include "sectors.h"

#include "global_limits.h"

///@brief An array to hold files found.
FILE_HEADER_REG* fileList[MAX_LISTABLE_FILES];

///@brief A function to compare two pointers to FILE_HEADER_REG pointers by their file name and extension contents (alphabetically).
///@param[in] file1 A pointer to a pointer to a FILE_HEADER_REG.
///@param[in] file2 A pointer to a pointer to a FILE_HEADER_REG.
///@return Returns an int value that is 0 if equal, <0 if less, and >0 if greater.
int compareFileHeaderByName(const FILE_HEADER_REG **file1, const FILE_HEADER_REG **file2)
{
	/*char str1[13];
	char str2[13];
	
	strcpy(str1, getFileNameStringFromFileHeader(*file1));
	strcpy(str2, getFileNameStringFromFileHeader(*file2));
	
	return strcmp(str1, str2);*/
	
	
	return memcmp((*file1)->file_name, (*file2)->file_name, 11);
}

///@brief A function to print a file's information for ls.
///@param[in] header A FILE_HEADER_REG pointer to a file, the contents of which to be displayed.
void listFileEntry(FILE_HEADER_REG *header)
{
	//Get file date.
	char date[33];
	getHumanReadableDateTimeString(&header->last_write_date, &header->last_write_time, date);
	
	char data[12];
	
	memcpy(data, header->file_name, 11);
	
	data[11] = NULL;
	
	for(uint32_t i = 0; i < 8; ++i)
	{
		if(data[i] == ' ')
			data[i] = NULL;
	}
	
	for(uint32_t i = 8; i < 11; ++i)
	{
		if(data[i] == ' ')
			data[i] = NULL;;
	}
	
	char *name = data;
	
	char *ext = &data[8];
	
	char size[9];
	
	if(header->attributes & FILE_ATTR_SUBDIRECTORY)
	{
		strcpy(size,"DIR");
	}
	else
	{
		sprintf(size,"%u",header->file_size);
	}
	
	printf("%u\t%s\t%s\t%s", (unsigned int)(header->first_logical_cluster), size, date, name);
	
	if(strlen(ext) > 0)
		printf(".%s", ext);
	
	//printf("\t%u", (unsigned int)(header->attributes));
	
	putchar('\n');
}

///@brief Main function for ls.
///@test If ls is called with no arguments, ls shall list the files and folders of the current working directory, providing their individual FLCs, sizes, dates, and names.
///@test If ls is called with an argument that is a valid path to a directory, ls shall list the files and folders of the provided directory, displaying their individual FLCs, sizes, dates, and names.
///@test If ls is called with an argument that is a valid path to a file, ls shall print the listing for that individual file, displaying its FLC, size, date, and name.
///@test If ls is called with an argument that is an invalid path to a directory, ls shall exit printing, "Could not find path".
///@test If the arguments provided to ls number more than one, ls shall exit printing, "Too many arguments!".
///@test Any and all file/folder listings provided by ls shall be sorted in alphabetical order by file name and extension if applicable.
///@test ls shall not print any file whose attributes are 0 or are 0x0f under any circumstances.
int ls_main(int argc, char* argv[])
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
	
	bool found = false;
	
	if(!path)
	{
		//printf("getting sharedmem %p\n", getDirStackTop(sharedMem));
		thisDir = getDirStackTop(sharedMem);
	}
	else
	{
		//thisDir = (FILE_HEADER*)findFile(path, getDirStackTop(sharedMem));
		
		found = findFile(path, getDirStackTop(sharedMem), &thisDir);
		
		if(!thisDir)
		{
			printf("Could not find path\n");
			exit(1);
		}	
	}
	
	//printf("This dir is: %p\n", thisDir);
	
	printf("\nFLC:\tSize:\tLast Write Timestamp:\t\t\tName:\n");
	printf("------------------------------------------------------------\n");
	
	FILE_HEADER_REG *currentHeader = NULL;
	
	//Check if we are in root.
	if((thisDir == NULL && found) || (!path && sharedMem->stack_top_index == 0))
	{
		//printf("CHECKING ROOT\n");
		
		//Set currentHeader to root;
		currentHeader = (FILE_HEADER_REG*)find_sector(ROOT_OFFSET);//&thisDir->header;
		
		for (int i = 0; i < MAX_FILES_IN_ROOT_DIR; i++)
		{
			if (currentHeader->attributes != 0 && (unsigned char)currentHeader->file_name[0] != FILE_DELETED_BYTE && currentHeader->attributes != 0x0f)//Is this an invalid entry or a long file name?
			{
				//if((currentHeader->attributes & FILE_ATTR_HIDDEN) != 0 && *(unsigned char*)(currentHeader) != FILE_DELETED_BYTE)
				//{
					//Add file to the list.
					fileList[fileIndex++] = currentHeader;
					//listFileEntry(currentHeader);
				//}
			}
			
			currentHeader++;
		}
	}
	else if((thisDir->header.attributes & FILE_ATTR_SUBDIRECTORY) != 0)
	{	
			unsigned char* fat = (unsigned char*)find_sector(FAT1_OFFSET);
			uint16_t currentCluster = thisDir->header.first_logical_cluster;
			
			do
			{
				FILE_HEADER_REG* dataSector = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentCluster);
				currentHeader = dataSector;
				
				for (int i = 0; i < 16; i++)
				{
					if (currentHeader->attributes != 0x0f && (unsigned char)currentHeader->file_name[0] != FILE_DELETED_BYTE && currentHeader->attributes != 0)//Is this a long file name?
					{
						//if((currentHeader->attributes & FILE_ATTR_HIDDEN) != 0 && *(unsigned char*)(currentHeader) != FILE_DELETED_BYTE)
						//{
							//Add file to the list.
							fileList[fileIndex++] = currentHeader;
							
							//listFileEntry(currentHeader);
							
							//printFileHeader(currentHeader);
						//}
					}
					
					currentHeader++;
				}
				
				currentCluster = get_fat_entry(currentCluster, fat);
			} while (currentCluster < 0xFF7);//Loop through all subdirectory sectors in chain			
	}
	else
	{
		listFileEntry(thisDir);
		exit(1);
	}
	
	//Sort by name.
	qsort(fileList, fileIndex, sizeof(FILE_HEADER_REG*), compareFileHeaderByName);
	
	//Now we can display.
	
	for(size_t i = 0; i < fileIndex; ++i)
	{
		listFileEntry(fileList[i]);
	}
	
	printf("\n%u files found\n", fileIndex);
	
	return 0;
}

int main(int argc, char* argv[])
{
	return ls_main(argc, argv);
}