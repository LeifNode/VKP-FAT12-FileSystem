#include "cat.h"

void cat(const FILE_HEADER_REG* file)
{
	void* buffer = NULL;

	//Could just substitute this for reading sector by sector and printing
	//Not really needed, but if we were going to be keeping RAM used to a minimum we might want to
	readFile(file, &buffer);

	for (uint64_t i = 0; i < file->file_size; i++)
	{
		printf("%c", *(char*)((uint64_t)buffer + i));
	}

	free(buffer);
}