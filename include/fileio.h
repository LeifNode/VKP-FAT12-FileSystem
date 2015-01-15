#ifndef FILEIO_H
#define FILEIO_H

#include "common.h"

#pragma pack(push)
#pragma pack(8)

typedef struct FILE_HEADER
{
  char            file_name[8];
  char            extension[3];
  
  uint8_t         attributes : 8;
  
  uint16_t        reserved : 16;
  
  uint16_t        creation_time : 16;
  uint16_t        creation_date : 16;
  uint16_t        last_access_date : 16;
  
  uint16_t        ignore : 16;

  uint16_t        last_write_time : 16;
  uint16_t        last_write_date : 16;
  
  uint16_t        first_logical_cluster : 16;
  uint32_t        file_size : 32;
} FILE_HEADER;

#pragma pack(pop)

typedef enum FILE_ATTRIBUTE
{
  FILE_ATTRIBUTE_READONLY     = 1,
  FILE_ATTRIBUTE_HIDDEN       = 1 << 1,
  FILE_ATTRIBUTE_SYSTEM       = 1 << 2,
  FILE_ATTRIBUTE_VOLUME_LABEL = 1 << 3,
  FILE_ATTRIBUTE_SUBDIRECTORY = 1 << 4,
  FILE_ATTRIBUTE_ARCHIVE      = 1 << 5
} FILE_ATTRIBUTE;

void printFileHeader(const FILE_HEADER* header);

#endif
