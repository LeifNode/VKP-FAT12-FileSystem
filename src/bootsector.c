#include "bootsector.h"

extern uint8_t *FILE_SYSTEM;

BOOT_SECTOR PBS_BOOT_SEC = {0};
uint16_t BYTES_PER_SECTOR = 0;

void readBootSector()
{
    PBS_BOOT_SEC = *(BOOT_SECTOR*)FILE_SYSTEM;
    BYTES_PER_SECTOR = PBS_BOOT_SEC.bytes_per_sector;
}

BOOT_SECTOR* getBootSector(uint8_t* fileSystem)
{
    return (BOOT_SECTOR*)fileSystem;
}

void printBootSector(BOOT_SECTOR* bootSector)
{
    printf("Bytes per sector             = %d\n", bootSector->bytes_per_sector);
    printf("Sectors per cluster          = %d\n", bootSector->sectors_per_cluster);
    printf("Number of FATs               = %d\n", bootSector->number_of_FATs);
    printf("Number of reserved sectors   = %d\n", bootSector->number_of_reserved_sectors);
    printf("Max number of root entries   = %d\n", bootSector->max_root_dir_entries);
    printf("Total sector count           = %d\n", bootSector->total_sector_count);
    printf("Sectors per FAT              = %d\n", bootSector->sectors_per_FAT);
    printf("Sectors per track            = %d\n", bootSector->sectors_per_track);
    printf("Number of heads              = %d\n", bootSector->number_of_heads);
    printf("Boot signiture (in hex)      = 0x%x\n", bootSector->boot_signature);
    printf("Volume ID (in hex)           = 0x%x\n", bootSector->volume_id);
	printf("OEM Name                     = ");
	
    for (int i = 0; i < 8 || bootSector->OEM_name[i] == '\0'; i++)
    {
        printf("%c", bootSector->OEM_name[i]);
    }

	printf("\nVolume label                 = ");
	
    for (int i = 0; i < 11 || bootSector->volume_label[i] == '\0'; i++)
    {
        printf("%c", bootSector->volume_label[i]);
    }
    
    printf("\nFile system type             = ");

    for (int i = 0; i < 8 || bootSector->file_system_type[i] == '\0'; i++)
    {
        printf("%c", bootSector->file_system_type[i]);
    }

    printf("\n");
}
