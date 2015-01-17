#include "header.h"
#include "pfe.h"
#include "cat.h"

#ifdef _WIN32

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
	if (hImage)
	{
		CloseHandle(hImage);
	}

	//Open our file through Windows API with required permissions.
	hImage = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	assert(hImage != INVALID_HANDLE_VALUE);

	//Check that we don't already have a memory map object open.
	if (hMap)
	{
		CloseHandle(hMap);
	}

	//Create a map object.
	hMap = CreateFileMapping(hImage, NULL, PAGE_READWRITE, 0, 0, NULL);

	//Get Image Size.
	DWORD fileSize = GetFileSize(hImage, NULL);

	//Check that our memory mapped pointer isn't already allocated.
	if (FILE_SYSTEM)
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

void closeFileSystem()
{
#ifdef WIN32
	if (FILE_SYSTEM)
	{
		UnmapViewOfFile(FILE_SYSTEM);
		FILE_SYSTEM = NULL;
	}

	if (hMap)
	{
		CloseHandle(hMap);
		hMap = NULL;
	}

	if (hImage)
	{
		CloseHandle(hImage);
		hImage = NULL;
	}
#endif
}

unsigned char test[] = { 0x00, 0x0F, 0xFF, 0x00, 0x0F, 0xFF };

int main(int argc, char *argv[])
{
    openFileSystem("./data/floppy1");
    readBootSector();
    printBootSector();

	printf("%d\n\n\n", get_fat_entry(2, test));

    unsigned char* fat_sector = allocSector();
	unsigned char* dir_sector = NULL;


    read_sector(1, fat_sector);
    
	//Print fat entries 2 through 20
	pfe(2, 20);

	dir_sector = find_sector(19);

	for (int i = 0; i < 16; i++)
	{
		printFileHeader(((FILE_HEADER*)dir_sector) + i);
		printf("\n");
	}

	FILE_HEADER* example_header = ((FILE_HEADER*)dir_sector) + 2;

	//Print out one of the example files from floppy1
	cat(example_header);

	/*unsigned char* mm = find_sector(31 + 2);
	for (int i = 0; i < 16; i++)
	{
		printFileHeader((FILE_HEADER*)(mm)+i);
	}
	free(mm);*/

	free(fat_sector);

	closeFileSystem();

    return 0;
}

