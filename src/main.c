#include "common.h"
#include "pbs.h"
#include "sectors.h"
#include "fat.h"
#include "fileio.h"

#include <cassert>

#ifdef _WIN32

#include <Windows.h>

//Handle for memory map object.
HANDLE hMap = NULL;

//Handle for file object.
HANDLE hImage = NULL;

#endif

//Memory map array for file.
uint8_t *FILE_SYSTEM = NULL;

void openFileSystem(const char* path)
{
    //Open file and create memory map.
    //===========================

#ifndef _WIN32
	
    struct stat statbuf;

    int fd_in = open(path, O_RDONLY);

    fstat(fd_in, &statbuf);

    FILE_SYSTEM = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd_in, 0);

#else

	//Close image if already open.
	if(hImage)
	{
		CloseHandle(hImage);
	}

	//Open our file through Windows API with required permissions.
	hImage = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	assert(hImage != INVALID_HANDLE_VALUE);
	
	//Check that we don't already have a memory map object open.
	if(hMap)
	{
		CloseHandle(hMap);
	}
	
	//Create a map object.
	hMap = CreateFileMapping(hImage, NULL, PAGE_READWRITE, 0, 0, NULL);
	
	//Get Image Size.
	DWORD fileSize = GetFileSize(hImage, NULL);
	
	//Check that our memory mapped pointer isn't already allocated.
	if(FILE_SYSTEM)
	{
		UnmapViewOfFile(FILE_SYSTEM);
		
		FILE_SYSTEM = NULL;
	}
	
	//Create memory map.
	FILE_SYSTEM = (uint8_t*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
	assert(FILE_SYSTEM != NULL);
	
	
#endif

    //===========================
}

void CloseFileSystem()
{
#ifdef WIN32
	if(FILE_SYSTEM)
	{
		UnmapViewOfFile(FILE_SYSTEM);
		FILE_SYSTEM = NULL;
	}
	
	if(hMap)
	{
		CloseHandle(hMap);
		hMap = NULL;
	}
	
	if(hImage)
	{
		CloseHandle(hImage);
		hImage = NULL;
	}
#endif
}

unsigned char test[] = { 0x00, 0x0F, 0xFF, 0x00, 0x0F, 0xFF };

uint8_t* allocSector()
{
	return malloc(BYTES_PER_SECTOR);
}

int main(int argc, char *argv[])
{
    openFileSystem("./img.dat");
    readBootSector();
    printBootSector();

	printf("%d\n\n\n", get_fat_entry(2, test));

    unsigned char* fat_sector = allocSector();
	unsigned char* dir_sector = NULL;


    read_sector(1, fat_sector);
    
    int entry_num = 2;
    for (entry_num = 2; entry_num < 20; entry_num++)
    {  
        printf("Fat entry #%d: %X\n", entry_num, get_fat_entry(entry_num, fat_sector));
    }
    
	//read_sector(19, dir_sector);

	dir_sector = find_sector(19);

	int i = 0;
	for (i = 0; i < 16; ++i)
	{
		FILE_HEADER *header = (FILE_HEADER*)(dir_sector)+i;

		if (memcmp((*header).file_name, "FOLD1", 5) == 0)
		{
			//header->file_name[0] = 'G';
			memcpy((void*)header->file_name, "FALD2", 5);
		}

		printFileHeader(header);
		printf("\n");
	}

	//free(dir_sector);

	/*uint8_t* ptr = find_sector(36, fat_sector);

	for (i = 0; i < 512; ++i)
	{
		printf("%c", ptr);

		ptr++;
	}*/

	/*uint8_t *mm = malloc(BYTES_PER_SECTOR);

	read_sector(33, mm);*/

	uint8_t *mm = find_sector(33);

	uint32_t g;
	for (g = 0; g < BYTES_PER_SECTOR; ++g)
	{
		printf("%c", mm[g]);

		//printFileHeader((FILE_HEADER*)(mm));
	}

	//free(mm);
	free(fat_sector);
	//free(dir_sector);

    return 0;
}

