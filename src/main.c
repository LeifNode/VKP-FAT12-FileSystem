#include "common.h"
#include "pbs.h"
#include "sectors.h"
#include "fat.h"

uint8_t *FILE_SYSTEM = 0;

void openFileSystem(const char* path)
{
    //Open file and create memory map.
    //===========================

    struct stat statbuf;

    int fd_in = open(path, O_RDONLY);

    fstat(fd_in, &statbuf);

    FILE_SYSTEM = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd_in, 0);

    //===========================
}

int main()
{
    openFileSystem("./data/floppy1");
    readBootSector();
    printBootSector();
    
    unsigned char* fat_sector = (unsigned char*)malloc(BYTES_PER_SECTOR);

    read_sector(1, fat_sector);
    
    int entry_num = 2;
    for (entry_num = 2; entry_num < 20; entry_num++)
    {  
        printf("Fat entry #%d: %X\n", entry_num, get_fat_entry(entry_num, fat_sector));
    }
    free(fat_sector);
    
    return 0;
}
