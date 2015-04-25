#include "fileio.h"
#include "fat.h"
#include "sectors.h"

#include "global_limits.h"

#include "sharedmemory.h"

#include <ctype.h>

extern uint8_t *FILE_SYSTEM;

char* getFileHeaderNameChunkFromFileNameString(char *filenameString)
{	
	static char buffer[11];
	
	if(filenameString == NULL)
	{
		return NULL;
	}
	
	//Pre-fill with spaces.
	memset(buffer, ' ', 11);
	
	if(filenameString[0] == '.')
	{
		if(strcmp(filenameString, "..") == 0 || strcmp(filenameString, ".") == 0)
		{
			for(int i = 0; i < strlen(filenameString); i++)
			{
				buffer[i] = filenameString[i];
			}
			
			return buffer;
		}
	}
	
	//Temporarily store a duplicate of input string to be mangled by strtok.
	char *tmp = strdup(filenameString);
	
	//Set size.
	uint32_t siz = strlen(tmp);
	
	//Make tmp uppercase.
	for(uint32_t i = 0; i < siz; ++i)
	{
		tmp[i] = toupper(tmp[i]);
	}
	
	char *saveptr;
	
	//Get tokenized string.
	char *pch = strtok_r(tmp, ".", &saveptr);
	
	//Set size.
	siz = strlen(pch);
	
	
	uint32_t i;
	
	//Start copying filename.
	for(i = 0; i < 8, i < siz; ++i)
	{
		//Don't!!!! Replace space with underscore.
		//if(pch[i] == ' ')
			//pch[i] = '_';
		
		//Copy a character.
		buffer[i] = pch[i];
	}
	
	//Get extension if extant.
	pch = strtok_r(NULL, ".", &saveptr);
	
	
	if(pch)
	{
		//Get extension length.
		siz = strlen(pch);
		
		//Start copying extension.
		for(i = 0; i < 3, i < siz; ++i)
		{
			buffer[i+8] = pch[i];
		}
	}
	
	free(tmp);
	
	return buffer;
}

char* getFileNameStringFromFileHeader(FILE_HEADER_REG *header)
{
	static char buffer[13];
	
	if(!header)
		return NULL;
	
	memset(buffer, NULL, 13);

	char *pos = header->file_name;
	
	char *out = buffer;
	
	uint32_t i;
	
	for(i = 0; i < 8; ++i)
	{
		if(*pos == ' ')
			break;
		
		*(out++) = *(pos++);
	}
	
	pos = header->extension;
	
	if(*pos != ' ')
		*(out++) = '.';
	
	for(i = 0; i < 3; ++i)
	{
		if(*pos == ' ')
			break;
		
		*(out++) = *(pos++);
	}
	
	return buffer;
}

void getNameFromLongNameFileHeader(const FILE_HEADER_LONGNAME *header, wchar_t *name)
{
	//This only works if wchar_t is 16 bits long.
	/*memcpy(name, header->name1, 5);
	memcpy(name + 5, header->name2, 6);
	memcpy(name + 11, header->name3, 2);*/

	for (size_t i = 0; i < 5; ++i)
	{
		name[i] = header->name1[i];
	}

	for (size_t i = 0; i < 6; ++i)
	{
		name[i+5] = header->name2[i];
	}

	for (size_t i = 0; i < 2; ++i)
	{
		name[i + 11] = header->name3[i];
	}

	//null terminate
	name[13] = 0x0000;
}

void printFileHeader(const FILE_HEADER* header)
{
	if (header->header.attributes == 0x0f)
	{
		printf("Index: %u\n", header->longname_header.index);
		printf("Long File name          = ");
		for (uint32_t i = 0; i < 5; ++i)
		{
			printf("%c", header->longname_header.name1[i]);
		}
		for (uint32_t i = 0; i < 6; ++i)
		{
			printf("%c", header->longname_header.name2[i]);
		}
		for (uint32_t i = 0; i < 2; ++i)
		{
			printf("%c", header->longname_header.name3[i]);
		}
		printf("\nType: %u\n", header->longname_header.type);
	}
	else
	{
		printf("File name               = ");

		for (int i = 0; i < 8; i++)
		{
			printf("%c", header->header.file_name[i]);
		}

		printf("\nExtension               = ");

		for (int i = 0; i < 3; i++)
		{
			printf("%c", header->header.extension[i]);
		}

		printf("\nAttributes              = %x\n", header->header.attributes);
		printf("First logical cluster   = %d\n", header->header.first_logical_cluster);
		printf("File size               = %dB\n", header->header.file_size);
	}
	
	struct tm timeptr;
	
	timeDateToCTime(&header->header.creation_date, &header->header.creation_time, &timeptr);
	
	printf("Creation timestamp: %s\n", asctime(&timeptr));
	
	timeDateToCTime(&header->header.last_access_date, NULL, &timeptr);
	
	printf("Last access date: %s\n", asctime(&timeptr));
	
	timeDateToCTime(&header->header.last_write_date, &header->header.last_write_time, &timeptr);
	
	printf("Last write timestamp: %s\n", asctime(&timeptr));
	
	putchar('\n');
}

void readFile(const FILE_HEADER* header, void** buffer)
{
	if (header->header.first_logical_cluster == 0)
	{
		printf("Invalid file header passed to readFile()\n");
		return;
	}
	if ((header->header.attributes & FILE_ATTR_SUBDIRECTORY) == 1)
	{
		printf("Directory header passed to readFile()\n");
		return;
	}

	void* fileBuffer = malloc(header->header.file_size);
	*buffer = fileBuffer;

	void* fat_sector = find_sector(1);

	uint16_t current_cluster = header->header.first_logical_cluster;
	uint16_t next_cluster = current_cluster;

	//How many sectors we have read so far
	uint16_t current_sector_offset = 0;

	do
	{
		current_cluster = next_cluster;
		next_cluster = get_fat_entry(current_cluster, (unsigned char*)fat_sector);

		uint32_t data_sector = current_cluster + DATA_OFFSET; //Offset into data area


		void* source_mem = find_sector(data_sector);
		//If we're at the last sector then only copy the memory required to reach the end of the file
		size_t mem_size = BYTES_PER_SECTOR;
		if (next_cluster >= 0xFF0)
			mem_size = header->header.file_size - (current_sector_offset * BYTES_PER_SECTOR);

		void* dest_mem = (void*)((uint64_t)fileBuffer + (uint64_t)BYTES_PER_SECTOR * (uint64_t)current_sector_offset);

		memcpy(dest_mem, source_mem, mem_size);

		current_sector_offset++;
	} while (next_cluster < 0xFF0); //While we're not reading the last valid cluster in the file
}

bool findFileInDir(const char* name, const FILE_HEADER* searchLocation, FILE_HEADER_REG** found)
{
	if(!name)
	{
		*found = NULL;
		return false;
	}
	
	/*if(name[0] = '/')
	{
		searchLocation = NULL;
	}*/
	
	///TODO: Check extensions 
	if (strlen(name) > MAX_PATH_SIZE)
	{
		printf("Specified file name \"%s\" is too long", name);
		*found = NULL;
		return false;
	}
	
	char *nextFileName = getFileHeaderNameChunkFromFileNameString(name);
	FILE_HEADER_REG* currentHeader = NULL;

	if (searchLocation == NULL) //Search root directory
	{
		FILE_HEADER_REG* root = (FILE_HEADER_REG*)find_sector(ROOT_OFFSET);
		currentHeader = root;
		
		for (int i = 0; i < MAX_FILES_IN_ROOT_DIR; i++)
		{
			if (currentHeader->attributes != 0x0f)//Is this a long file name?
			{				
				if (memcmp(nextFileName, currentHeader->file_name, 11) == 0)
				{
					*found = currentHeader;
					return true;
				}
			}
			
			currentHeader++;
		}
	}
	else //Search specified directory
	{
		unsigned char* fat = (unsigned char*)find_sector(FAT1_OFFSET);
		uint16_t currentCluster = searchLocation->header.first_logical_cluster;
		
		do
		{
			FILE_HEADER_REG* dataSector = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentCluster);
			currentHeader = dataSector;
			
			for (int i = 0; i < 16; i++)
			{
				if (currentHeader->attributes != 0x0f)//Is this a long file name?
				{	
					if (memcmp(nextFileName, currentHeader->file_name, 11) == 0)
					{
						*found = currentHeader;
						
						if(isRoot(*found))
							*found = NULL;
						
						return true;
					}
				}
				
				currentHeader++;
			}
			
			currentCluster = get_fat_entry(currentCluster, fat);
		} while (currentCluster < 0xFF7);//Loop through all subdirectory sectors in chain
	}
	
	return false;
}

bool findFile(const char* name, const FILE_HEADER* searchLocation, FILE_HEADER_REG** found)
{
	*found = NULL;
	
	//If no name given, then leave.
	if(!name)
	{
		return false;
	}
	
	//Check if the path given should be ignored and set to root automatically set to something else.
	if(name[0] == '/')
	{
		searchLocation = NULL;
	}
	
	//A local variable to hold the pointer to the next file to be found in the loop below and ultimately the file/directory searched for, if found.
	FILE_HEADER_REG *nextFile = NULL;
	
	char *path = strdup(name);
	
	for(uint32_t i = 0; i < strlen(path); ++i)
	{
		path[i] = toupper(path[i]);
	}
	
	char *saveptr;
	
	//A variable to hold the next file/directory to look for.
	char *nextFileName = strtok_r(path, "/", &saveptr);
	
	//Keep tokenizing our path string.
	while(nextFileName != NULL)
	{
		//Check if it couldn't be found and display an error message.
		if(!findFileInDir(nextFileName, searchLocation, &nextFile))
		{
			//printf("Could not find file/directory: %s\n", nextFileName);
			
			free(path);
			
			return false;
		}
		
		if(strcmp(nextFileName, "..") == 0)
		{	
			//Check if root.
			if(nextFile->first_logical_cluster == 0x00)
			{	
				searchLocation = NULL;
				
				//Set found to null.
				nextFile = NULL;
			}
			
		}
		
		//Get the new next file/directory to look for.
		nextFileName = strtok_r(NULL, "/", &saveptr);
		
		//Set new search location.
		searchLocation = nextFile;
	}
	
	//We've found the file! Hurray!
	*found = nextFile;
	
	free(path);
	
	return true;
}

bool gotoFile(const char* name, const FILE_HEADER* searchLocation, FILE_HEADER_REG** found)
{
	//printf("I got the filename: %s\n", name);
	
	if(found)
	{
		//Preset our found pointer to null in case we don't find anything by default.
		*found = NULL;
	}
	
	//Backup our shared memory.
	SHELL_SHARED_MEMORY tempMem;
	
	//if name is null, exit function.
	if(!name)
		return false;
		
	//Back up the current memory to use as a buffer.
	memcpy(&tempMem, getSharedMemoryPtr(), sizeof(SHELL_SHARED_MEMORY));
	
	//Check if the path given should be ignored and set to root automatically set to something else.
	if(name[0] == '/')
	{
		searchLocation = NULL;
		
		//Reset the top index for our stack buffer.
		tempMem.stack_top_index = 0;
	}
	
	//Convert to uppercase
	char *path = strdup(name);
	
	for(uint32_t i = 0; i < strlen(path); ++i)
	{
		path[i] = toupper(path[i]);
	}
	
	//A local variable to hold the pointer to the next file to be found in the loop below and ultimately the file/directory searched for, if found.
	FILE_HEADER_REG *nextFile = NULL;
	
	char *saveptr;
	
	//A variable to hold the next file/directory to look for.
	char *nextFileName = strtok_r(path, "/", &saveptr);
	
	//Keep tokenizing our path string.
	while(nextFileName != NULL)
	{
		//Check if it couldn't be found and display an error message.
		if(!findFileInDir(nextFileName, searchLocation, &nextFile))
		{
			//printf("Could not find file/directory: %s\n", nextFileName);
			free(path);
			return false;
		}
		
		if((nextFile->attributes & FILE_ATTR_SUBDIRECTORY) == 0)
		{
			printf("%s is not a directory!\n",nextFileName);
			free(path);
			return false;
		}
		
		if(strcmp(nextFileName, "..") == 0)
		{
			//Move back a directory for path cosmetic reasons.
			popDirStack(&tempMem);
			
			//Check if root.
			if(nextFile->first_logical_cluster == 0x00)
			{
				//Reset the top index for our stack buffer.
				tempMem.stack_top_index = 0;
				
				searchLocation = NULL;
				
				//Set found to null.
				nextFile = NULL;
			}
			
		}
		else if(strcmp(nextFileName,".") == 0)
		{
			
		}
		else
		{
			//Since our file header is valid, push it onto the temporary file stack.
			pushDirStack(&tempMem, (FILE_HEADER*)nextFile);
		}
		
		//Get the new next file/directory to look for.
		nextFileName = strtok_r(NULL, "/", &saveptr);
		
		searchLocation = nextFile;
	}
	
	if(found)
	{
		//We've found the file! Hurray!
		*found = nextFile;
	}
	
	//Now copy everything back to the actual shared memory.
	memcpy(getSharedMemoryPtr(), &tempMem, sizeof(SHELL_SHARED_MEMORY));
	
	free(path);
	
	return true;
}

void deleteFile(FILE_HEADER* header)
{
	int flc = header->header.first_logical_cluster;
	
	memset(header, 0, sizeof(FILE_HEADER));
	
	freeFatChain(flc, true);
}

int getDirectoryFreeEntryCount(FILE_HEADER* directory)
{
	if (directory == NULL ||
		directory->header.attributes & FILE_ATTR_SUBDIRECTORY == 0 ||
		directory->header.attributes == 0x0f)
	{
		return -1;
	}
	
	int maxDirectoryEntries = 0;
	int freeEntryCount = 0;
	
	int currentSector = directory->header.first_logical_cluster;
	unsigned char* fat = (unsigned char*)find_sector(FAT1_OFFSET);
	
	do
	{
		maxDirectoryEntries += 16;
		
		FILE_HEADER_REG* currentDirEntry = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentSector);
		
		for (int i = 0; i < 16; i++)
		{
			if (currentDirEntry[i].attributes == 0 && currentDirEntry[i].first_logical_cluster == 0)
			{
				freeEntryCount++;
			}
		}
		
		currentSector = get_fat_entry(currentSector, fat);
	} while(currentSector < 0xFF7);
	
	return freeEntryCount;
}

void collapseDirectory(FILE_HEADER* directory)
{
	int currentSector = directory->header.first_logical_cluster;
	
	int currentFileIndex = 0;
	int currentSectorIndex = 0;
	
	int freeFileIndex = -1;
	FILE_HEADER_REG* freeEntry = NULL;
	
	unsigned char* fat1 = (unsigned char*)find_sector(FAT1_OFFSET);
	unsigned char* fat2 = (unsigned char*)find_sector(FAT2_OFFSET);

	//Shift all entries down into free spaces
	do
	{
		FILE_HEADER_REG* currentDirEntry = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentSector);
		
		for (int i = 0; i < 16; i++)
		{
			//Find next free sector
			if (freeFileIndex <= currentFileIndex && 
			    (currentDirEntry[i].attributes != 0 || currentDirEntry[i].first_logical_cluster != 0))//Save needless work on re-finding the same entry
			{
				//This should probably be factored out into its own function at some point
				//Loops through the directory and finds the first free directory entry
				
				unsigned int currentFreeSector = currentSector; //We can assume that every sector before the current one is already filled
				freeFileIndex = currentSectorIndex * 16;
				
				do
				{
					FILE_HEADER_REG* freeDirEntry = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentFreeSector);
					
					if (freeEntry != NULL)
						break;
					
					for (int freei = 0; freei < 16; freei++)
					{
						if (freeDirEntry[freei].attributes == 0 && freeDirEntry[freei].first_logical_cluster == 0)//If the entry is used
						{
							freeEntry = &freeDirEntry[freei];
							
							//Make sure it's completely clear
							memset(&freeDirEntry[freei], 0, sizeof(FILE_HEADER));
							
							break;
						}
						
						freeFileIndex++;
					}
					
					currentFreeSector = get_fat_entry(currentFreeSector, fat1);
				} while(currentFreeSector < 0xFF7);
			}
			
			if ((currentDirEntry[i].attributes != 0 || currentDirEntry[i].first_logical_cluster != 0) &&
			    freeFileIndex > 0 && freeFileIndex < currentFileIndex && freeEntry != NULL)
			{
				memcpy(freeEntry, &currentDirEntry[i], sizeof(FILE_HEADER));
				memset(&currentDirEntry[i], 0, sizeof(FILE_HEADER));
				
				freeEntry = NULL;
				freeFileIndex = -1;
			}
			
			currentFileIndex++;
		}
		
		currentSector = get_fat_entry(currentSector, fat1);
		currentSectorIndex++;
	} while(currentSector < 0xFF7);
	
	//Delete unused sectors of the directory
	
	unsigned int lastUsedSector = 0;
	
	currentSector = directory->header.first_logical_cluster;
	currentSector = get_fat_entry(currentSector, fat1); //We want to always keep the first sector of the directory so we skip it
	
	while(currentSector < 0xFF7) //Find last used sector
	{
		FILE_HEADER_REG* currentDirEntry = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentSector);
		
		for (int i = 0; i < 16; i++)
		{
			if (currentDirEntry[i].attributes != 0 || currentDirEntry[i].first_logical_cluster != 0)
			{
				lastUsedSector = currentSector;
			}
		}
		
		currentSector = get_fat_entry(currentSector, fat1);
	}
	
	if (lastUsedSector != 0xFFF)//Free all sectors following the last used one
	{
		freeFatChain(get_fat_entry(lastUsedSector, fat1), true);
		
		set_fat_entry(lastUsedSector, 0xFFF, fat1);
		set_fat_entry(lastUsedSector, 0xFFF, fat2);
	}
}

FILE_HEADER_REG* getNextFreeDirectoryEntry(FILE_HEADER* directory)
{
	if (directory == NULL) //Root
	{
		FILE_HEADER_REG* currentDirEntry = (FILE_HEADER_REG*)find_sector(ROOT_OFFSET);
		
		for (int i = 0; i < 224; i++)
		{
			if (currentDirEntry[i].attributes == 0 && currentDirEntry[i].first_logical_cluster == 0)
			{
				return &currentDirEntry[i];
			}
		}
		
		return NULL;
	}
	else //Subdirectory
	{
		if (directory->header.attributes & FILE_ATTR_SUBDIRECTORY == 0 ||
			directory->header.attributes == 0x0f)
		{
			return NULL;
		}
		
		int currentSector = directory->header.first_logical_cluster;
		int lastSector = currentSector;
		unsigned char* fat = (unsigned char*)find_sector(FAT1_OFFSET);
		
		do
		{
			FILE_HEADER_REG* currentDirEntry = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentSector);
			
			for (int i = 0; i < 16; i++)
			{
				if (currentDirEntry[i].attributes == 0 && currentDirEntry[i].first_logical_cluster == 0)
				{
					return &currentDirEntry[i];
				}
			}
			
			lastSector = currentSector;
			currentSector = get_fat_entry(currentSector, fat);
		} while(currentSector < 0xFF7);
		
		unsigned int newSector = appendSector(lastSector);
		
		if (newSector == 0xFFF)
			return NULL;
			
		return (FILE_HEADER_REG*)find_sector(DATA_OFFSET + newSector);
	}
}

bool isDirectoryEmpty(FILE_HEADER* directory)
{
	if (directory == NULL ||
		directory->header.attributes & FILE_ATTR_SUBDIRECTORY == 0 ||
		directory->header.attributes == 0x0f)
	{
		return false;
	}
	
	int currentSector = directory->header.first_logical_cluster;
	unsigned char* fat = (unsigned char*)find_sector(FAT1_OFFSET);
	
	do
	{
		FILE_HEADER_REG* currentDirEntry = (FILE_HEADER_REG*)find_sector(DATA_OFFSET + currentSector);
		
		for (int i = 0; i < 16; i++)
		{
			if (currentDirEntry[i].first_logical_cluster != 0)
			{
				if (memcmp(currentDirEntry[i].file_name, ".   ", 4) != 0 &&
					memcmp(currentDirEntry[i].file_name, "..  ", 4) != 0)
				{
					return false;
				}
			}
		}
		
		currentSector = get_fat_entry(currentSector, fat);
	} while(currentSector < 0xFF7);
	
	return true;
}

void cat(const FILE_HEADER_REG* file)
{
	void* buffer = NULL;

	//Could just substitute this for reading sector by sector and printing
	//Not really needed, but if we were going to be keeping RAM used to a minimum we might want to
	readFile((FILE_HEADER*)file, &buffer);

	for (uint64_t i = 0; i < file->file_size; i++)
	{
		printf("%c", *(char*)((uint64_t)buffer + i));
	}

	free(buffer);
}

bool isRoot(void* file)
{
	return file == (void*)find_sector(ROOT_OFFSET);
}