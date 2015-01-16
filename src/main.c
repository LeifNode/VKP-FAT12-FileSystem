#include "header.h"

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
    
    int entry_num = 2;
    for (entry_num = 2; entry_num < 20; entry_num++)
    {  
        printf("Fat entry #%d: %X\n", entry_num, get_fat_entry(entry_num, fat_sector));
    }
    
	//read_sector(19, dir_sector);

	dir_sector = find_sector(19);

	


	/*uint8_t* ptr = find_sector(36, fat_sector);

	for (i = 0; i < 512; ++i)
	{
		printf("%c", ptr);

		ptr++;
	}*/

	/*uint8_t *mm = malloc(BYTES_PER_SECTOR);

	read_sector(33, mm);*/

	unsigned char* mm = find_sector(33);

	uint32_t g;
	for (g = 0; g < BYTES_PER_SECTOR; ++g)
	{
		printf("%c", mm[g]);

		//printFileHeader((FILE_HEADER*)(mm));
	}

	//free(mm);
	free(fat_sector);
	//free(dir_sector);

	closeFileSystem();

    return 0;
}

