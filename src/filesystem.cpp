#include "filesystem.h"
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