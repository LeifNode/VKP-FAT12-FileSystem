#ifndef PBS_H
#define PBS_H

#include "common.h"

#pragma pack(push)
#pragma pack(1)

/// \struct BOOT_SECTOR
/// \brief A struct that stores boot sector information
typedef struct BOOT_SECTOR
{
  uint8_t         bootstrap_jump[3];

  char            OEM_name[8];

  uint16_t        bytes_per_sector : 16;
  uint8_t         sectors_per_cluster : 8;
  uint16_t        number_of_reserved_sectors : 16;
  uint8_t         number_of_FATs : 8;
  uint16_t        max_root_dir_entries : 16;
  uint16_t        total_sector_count : 16;

  uint8_t         ignore2 : 8;

  uint16_t        sectors_per_FAT : 16;
  uint16_t        sectors_per_track : 16;
  uint16_t        number_of_heads : 16;

  uint32_t        ignore3 : 32;

  uint32_t        FAT32_total_sector_count : 32;

  uint16_t        ignore4 : 16;

  uint8_t         boot_signature : 8;

  uint32_t        volume_id : 32;
  char            volume_label[11];

  char            file_system_type[8];
} BOOT_SECTOR ;

#pragma pack(pop)

extern BOOT_SECTOR PBS_BOOT_SEC;
extern uint16_t BYTES_PER_SECTOR;

/// \brief Reads the boot sector from sector 0 on the file system
///
void readBootSector();

BOOT_SECTOR* getBootSector(uint8_t* fileSystem);

/// \brief Prints the contents of the boot sector to stdout
///
/// \param[in]	bootSector	A pointer to a BOOT_SECTOR object holding the information to print.
void printBootSector(BOOT_SECTOR* bootSector);

#endif
