#include "fileio.h"
#include "fat.h"
#include "sectors.h"

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

		uint32_t data_sector = current_cluster + 31; //Offset into data area


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