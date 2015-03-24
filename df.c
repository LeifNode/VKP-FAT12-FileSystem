#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "sectors.h"

int main(int argc, char* argv[])
{	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	int freeSectorCount = 0;
	int usedSectorCount = 0;
	
	BOOT_SECTOR* boot_sector = &PBS_BOOT_SEC;
	void* fat_sector = find_sector(FAT1_OFFSET);
	
	int fatSectorCount = (boot_sector->total_sector_count - DATA_OFFSET - 2);

	for (int i = 2; i < fatSectorCount; i++)
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