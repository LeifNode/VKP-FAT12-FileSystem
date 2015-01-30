#include "common.h"
#include "sectors.h"
#include "bootsector.h"
#include "fileio.h"
#include "fat.h"



unsigned char test[] = { 0x00, 0x0F, 0xFF, 0x00, 0x0F, 0xFF };

int main(int argc, char *argv[])
{
    openFileSystem("./data/floppy1");
    readBootSector();
    printBootSector();

	printf("%d\n\n\n", get_fat_entry(2, test));

	unsigned char* dir_sector = NULL;
    
	//Print fat entries 2 through 20
	pfe(2, 20);

	dir_sector = find_sector(19);

	for (int i = 0; i < 16; i++)
	{
		printFileHeader(((FILE_HEADER*)dir_sector) + i);
		printf("\n");
	}

	FILE_HEADER_REG* example_header = ((FILE_HEADER*)dir_sector) + 2;

	//Print out one of the example files from floppy1
	cat(example_header);

	/*unsigned char* mm = find_sector(31 + 2);
	for (int i = 0; i < 16; i++)
	{
		printFileHeader((FILE_HEADER*)(mm)+i);
	}
	free(mm);*/

	closeFileSystem();

    return 0;
}

