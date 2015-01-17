#include "fileio.h"
#include "fat.h"
#include "sectors.h"

void printFileHeader(const FILE_HEADER* header)
{
    printf("File name               = ");
    
    for(int i = 0; i < 8; i++)
    {
        printf("%c", header->file_name[i]);
    }

    printf("\nExtension               = ");
    
    for(int i = 0; i < 8; i++)
    {
        printf("%c", header->extension[i]);
    }
    
    printf("\nAttributes              = %x\n", header->attributes);
    printf("First logical cluster   = %d\n", header->first_logical_cluster);
    printf("File size               = %dB\n", header->file_size); 
}

void readFile(const FILE_HEADER* header, void** buffer)
{
	if (header->first_logical_cluster == 0)
	{
		printf("Invalid file header passed to readFile()\n");
		return;
	}
	if ((header->attributes & FILE_ATTR_SUBDIRECTORY) == 1)
	{
		printf("Directory header passed to readFile()\n");
		return;
	}

	void* fileBuffer = malloc(header->file_size);
	*buffer = fileBuffer;

	void* fat_sector = find_sector(1);

	uint16_t current_cluster = header->first_logical_cluster;
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
			mem_size = header->file_size - (current_sector_offset * BYTES_PER_SECTOR);

		void* dest_mem = (void*)((uint64_t)fileBuffer + (uint64_t)BYTES_PER_SECTOR * (uint64_t)current_sector_offset);

		memcpy(dest_mem, source_mem, mem_size);

		current_sector_offset++;
	} while (next_cluster < 0xFF0); //While we're not reading the last valid cluster in the file
}