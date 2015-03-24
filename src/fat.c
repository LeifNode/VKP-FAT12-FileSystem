#include "fat.h"
#include "sectors.h"
#include "bootsector.h"

unsigned int get_fat_entry(int fat_entry_number, unsigned char* fat)
{
  int offset;
  int uv, wx, yz;

  offset = 3 * fat_entry_number / 2;

  // Two FAT12 entries are stored into three bytes;
  // if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw

  // odd fat entry number, return yzw
  if (fat_entry_number & 0x0001)
    {
      wx = (int) fat[offset];
      yz = (int) fat[offset + 1];
      return ( (yz << 4)  |  ( (wx & 0x00f0) >> 4));
    }
  // even fat entry number, return xuv
  else
    {
      uv = (int) fat[offset];
      wx = (int) fat[offset + 1];
      return ( ((wx & 0x000f) << 8)  |  uv );
    }
}

void set_fat_entry(int fat_entry_number, int value, unsigned char* fat)
{
  int offset;
  int a, b, c;

  offset = 3 * fat_entry_number / 2;

  // Two FAT12 entries are stored into three bytes;
  // if these bytes are uv,wx,yz then the two FAT12 entries are xuv and yzw
  // Let 0a,bc denote the fat_entry_number, written as two bytes (high and
  // low, respectively)

  a = value & 0x0f00;
  b = value & 0x00f0;
  c = value & 0x000f;

  // odd fat entry number, change yzw to abc, i.e.,
  if (fat_entry_number & 0x0001)
    {
      // wx = cx;
      fat[offset]     = (unsigned char) ((c << 4)  |  (fat[offset] & 0x000f));
      // yz = ab;
      fat[offset + 1] = (unsigned char) ((a >> 4)  |  (b >> 4));
    }
  // even fat entry number, change xuv to abc, i.e.,
  else
    {
      // uv = bc;
      fat[offset]     = (unsigned char) (b | c);
      // wx = wa;
      fat[offset + 1] = (unsigned char) ((fat[offset + 1]  &
                                          0x00f0)  |  (a >> 8));
    }
}

uint16_t get_free_sector_count()
{
	void* fat_sector = find_sector(1);

	int totalUsed = 0;

	//Number of FAT entries per FAT is 384 in theory
	for (int i = 2; i < 384; i++)
	{
		if (get_fat_entry(i, fat_sector) == 0x000)
		{
			totalUsed++;
		}
	}

	return totalUsed;
}

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

	void* fat_sector = find_sector(FAT1_OFFSET);

	for (int i = start; i <= end; i++)
	{
		printf("Fat entry #%d: %X\n", i, get_fat_entry(i, fat_sector));
	}
}

void freeFatChain(int fatStart, bool zeroMemory)
{
	if (fatStart == 0)
		return;

	unsigned char* fat1 = (unsigned char*)find_sector(FAT1_OFFSET);
	unsigned char* fat2 = (unsigned char*)find_sector(FAT2_OFFSET);
	
	unsigned int currentEntry = fatStart;
	unsigned int nextEntry;

	do
	{
		nextEntry = get_fat_entry(currentEntry, fat1);
		
		if (zeroMemory && currentEntry != 0)
		{
			memset(find_sector(DATA_OFFSET + currentEntry), 0, BYTES_PER_SECTOR);
		}
		
		set_fat_entry(currentEntry, 0, fat1);
		set_fat_entry(currentEntry, 0, fat2);
		
		currentEntry = nextEntry;
	} while (nextEntry < 0xFF7);
}