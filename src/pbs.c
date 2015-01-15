#include "pbs.h"

extern uint8_t *FILE_SYSTEM;

BOOT_SECTOR PBS_BOOT_SEC = {0};
uint16_t BYTES_PER_SECTOR = 0;

void readBootSector()
{
    PBS_BOOT_SEC = *(BOOT_SECTOR*)FILE_SYSTEM;
    BYTES_PER_SECTOR = PBS_BOOT_SEC.bytes_per_sector;
}

void printBootSector()
{
    printf("Bytes per sector             = %d\n", PBS_BOOT_SEC.bytes_per_sector);
    printf("Sectors per cluster          = %d\n", PBS_BOOT_SEC.sectors_per_cluster);
    printf("Number of FATs               = %d\n", PBS_BOOT_SEC.number_of_FATs);
    printf("Number of reserved sectors   = %d\n", PBS_BOOT_SEC.number_of_reserved_sectors);
    printf("Max number of root entries   = %d\n", PBS_BOOT_SEC.max_root_dir_entries);
    printf("Total sector count           = %d\n", PBS_BOOT_SEC.total_sector_count);
    printf("Sectors per FAT              = %d\n", PBS_BOOT_SEC.sectors_per_FAT);
    printf("Sectors per track            = %d\n", PBS_BOOT_SEC.sectors_per_track);
    printf("Number of heads              = %d\n", PBS_BOOT_SEC.number_of_heads);
    printf("Boot signiture (in hex)      = 0x%x\n", PBS_BOOT_SEC.boot_signature);
    printf("Volume ID (in hex)           = 0x%x\n", PBS_BOOT_SEC.volume_id);
    printf("Volume label                 = ");

    int i = 0;
    for (i = 0; i < 11 || PBS_BOOT_SEC.volume_label[i] == '\0'; i++)
    {
        printf("%c", PBS_BOOT_SEC.volume_label[i]);
    }
    
    printf("\nFile system type             = ");

    for (i = 0; i < 8 || PBS_BOOT_SEC.file_system_type[i] == '\0'; i++)
    {
        printf("%c", PBS_BOOT_SEC.file_system_type[i]);
    }

    printf("\n");
}
