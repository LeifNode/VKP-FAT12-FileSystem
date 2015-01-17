#include "pfe.h"
#include "sectors.h"
#include "fat.h"

void pfe(int start, int end)
{
	//Ignoring dealing with a maximum now.. since i'm not sure what it is
	//Each cluster is 512B wide and there are 9 dedicated to each FAT table so the max should be 384 entries per table since each entry is 12B right?
	//If that's the case why are there 2847 data clusters?
	if (start < 2)
	{
		printf("Invalid input for pfe. Start entry must be at least 2.\n");
		return;
	}

	void* fat_sector = find_sector(1);

	for (int i = start; i <= end; i++)
	{
		printf("Fat entry #%d: %X\n", i, get_fat_entry(i, fat_sector));
	}
}