#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "sectors.h"

///@brief Main function for df command
///@test If called with any number of arguments, df shall display the number of K-blocks as well as the FAT sector count, the used sector count, the free sector count, and the percentage of sectors used.
int df_main(int argc, char* argv[])
{	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	int freeSectorCount = 0;
	int usedSectorCount = 0;
	
	BOOT_SECTOR* boot_sector = &PBS_BOOT_SEC;
	void* fat_sector = find_sector(FAT1_OFFSET);
	
	int fatSectorCount = (boot_sector->total_sector_count - DATA_OFFSET - 2);
	//int maxSector = (boot_sector->bytes_per_sector * boot_sector->sectors_per_fat * 8) / 12;
	
	for (int i = 2; i < fatSectorCount + 2; i++)
	{
		if (get_fat_entry(i, fat_sector) == 0)
		{
			freeSectorCount++;
		}
		else
		{
			usedSectorCount++;
		}
	}
	
	float percentUsed = (usedSectorCount / (float)fatSectorCount) * 100.0f;
	
	printf("%iK-blocks\tUsed\tAvailable\tUse %\n", boot_sector->bytes_per_sector);
	printf("%i\t\t%i\t%i\t\t%.2f\n", fatSectorCount, usedSectorCount, freeSectorCount, percentUsed);

	return 0;
}

int main(int argc, char* argv[])
{
	return df_main(argc, argv);
}