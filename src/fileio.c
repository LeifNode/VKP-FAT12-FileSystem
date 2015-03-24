#include "fileio.h"
#include "fat.h"
#include "sectors.h"

#include "global_limits.h"

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

FILE_HEADER_REG* findFile(const char* name, const FILE_HEADER* searchLocation)
{
	///TODO: Check extensions 
	if (strlen(name) > 8)
	{
		printf("Specified file name \"%s\" is too long", name);
		return NULL;
	}
	
	char fileName[9];
	FILE_HEADER_REG* currentHeader = NULL;

	if (searchLocation == NULL) //Search root directory
	{
		FILE_HEADER_REG* root = (FILE_HEADER_REG*)find_sector(ROOT_OFFSET);
		currentHeader = root;
		
		for (int i = 0; i < MAX_FILES_IN_ROOT_DIR; i++)
		{
			if (currentHeader->attributes != 0x0f)//Is this a long file name?
			{
				memset(fileName, 0, 9);//Clear name
				//Replace spaces in file name with null characters so we can compare names correctly
				for (int c = 0; c < 8; c++) 
				{
					if (currentHeader->file_name[c] == ' ')
					{
						fileName[c] = '\0';
						break;
					}
				
					fileName[c] = currentHeader->file_name[c];
				}
				
				if (strcmp(name, fileName) == 0)
				{
					return currentHeader;
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
					memset(fileName, 0, 9);//Clear name
					
					//Replace spaces in file name with null characters so we can compare names correctly
					//This could be moved to a function at some point
					for (int c = 0; c < 8; c++) 
					{
						if (currentHeader->file_name[c] == ' ')
						{
							fileName[c] = '\0';
							break;
						}
					
						fileName[c] = currentHeader->file_name[c];
					}
					
					if (strcmp(name, fileName) == 0)
					{
						return currentHeader;
					}
				}
				
				currentHeader++;
			}
			
			currentCluster = get_fat_entry(currentCluster, fat);
		} while (currentCluster < 0xFF7);//Loop through all subdirectory sectors in chain
	}
	
	return NULL;
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

int isRoot(const FILE_HEADER* file)
{
	return file == (FILE_HEADER*)find_sector(ROOT_OFFSET);
}