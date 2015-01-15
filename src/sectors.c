#include "sectors.h"
#include "string.h"

extern uint8_t *FILE_SYSTEM;


int read_sector(int sector_number, unsigned char* buffer)
{
  int bytes_read;

  /*if (fseek(FILE_SYSTEM_ID,
	    (long) sector_number * (long) BYTES_PER_SECTOR, SEEK_SET) != 0)
    {
      printf("Error accessing sector %d\n", sector_number);
      return -1;
    }

  bytes_read = fread(buffer, sizeof(char), BYTES_PER_SECTOR, FILE_SYSTEM_ID);

  if (bytes_read != BYTES_PER_SECTOR)
    {
      printf("Error reading sector %d\n", sector_number);
      return -1;
    }*/

  bytes_read = sizeof(uint8_t) * BYTES_PER_SECTOR;
  memcpy(buffer, (void*)((long)FILE_SYSTEM + (long)sector_number * (long)BYTES_PER_SECTOR), bytes_read);

  return bytes_read;
}


int write_sector(int sector_number, unsigned char* buffer)
{
  int bytes_written;

  //Using mmap
  /*  if (fseek(FILE_SYSTEM,
	    (long) sector_number * (long) BYTES_PER_SECTOR, SEEK_SET) != 0)
    {
      printf("Error accessing sector %d\n", sector_number);
      return -1;
      }*/

  bytes_written = sizeof(uint8_t) * BYTES_PER_SECTOR;
  memcpy((void*)(FILE_SYSTEM + sector_number * BYTES_PER_SECTOR), buffer, bytes_written);

  /*if (bytes_written != BYTES_PER_SECTOR)
    {
      printf("Error reading sector %d\n", sector_number);
      return -1;
      }*/

  return bytes_written;
}

uint8_t* find_sector(uint32_t sector_number)
{
	return (void*)((long)FILE_SYSTEM + (long)sector_number * (long)BYTES_PER_SECTOR);
}