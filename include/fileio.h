#ifndef FILEIO_H
#define FILEIO_H

#include "common.h"

#pragma pack(push)
#pragma pack(1)

///@brief A OS-independent type to store 16-bit characters.
typedef uint16_t char16_t;

///@brief A struct to store and manipulate long name file headers.
typedef struct FILE_HEADER_REG
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
} FILE_HEADER_REG;

///@brief A struct to store and manipulate long name file headers.
typedef struct FILE_HEADER_LONGNAME
{
	uint8_t         index;

	char16_t         name1[5];

	uint8_t         attributes;
	uint8_t         type;
	uint8_t         checksum;

	char16_t         name2[6];

uint16_t:	16;

	char16_t         name3[2];
} FILE_HEADER_LONGNAME;

///@brief A union of the regular 8.1 file header and the long name file header.
typedef union FILE_HEADER
{
	FILE_HEADER_REG         header;
	FILE_HEADER_LONGNAME    longname_header;
} FILE_HEADER;

#pragma pack(pop)

///@brief An enumeration to hold file attribute flags.
typedef enum FILE_ATTRIBUTE
{
	FILE_ATTR_READONLY = 1,
	FILE_ATTR_HIDDEN = 1 << 1,
	FILE_ATTR_SYSTEM = 1 << 2,
	FILE_ATTR_VOLUME_LABEL = 1 << 3,
	FILE_ATTR_SUBDIRECTORY = 1 << 4,
	FILE_ATTR_ARCHIVE = 1 << 5
} FILE_ATTRIBUTE;

///@brief Takes a pointer to a wide character string (at least 13 characters allocated) and populates it with the filename from a longname file header.
///@param [in]	header	A pointer to a FILE_HEADER_LONGNAME object.
///@param [out]	name	A pointer to a wchar_t string (32-bits per char on Linux, 16-bits per char on Windows)
void getNameFromLongNameFileHeader(const FILE_HEADER_LONGNAME *header, wchar_t *name);

///@brief Prints out the contents of a file header to a human-readable form in the console.
///@param [in]	header	A pointer to a FILE_HEADER union. (This could be either a FILE_HEADER_REG or a FILE_HEADER_LONGNAME.)
void printFileHeader(const FILE_HEADER* header);

///@brief Reads the contents of a file into a function-allocated buffer given a pointer to its file header and a pointer to store the buffer at.
///@param [in]	header	A pointer to a FILE_HEADER_REG object.
///@param [out]	buffer	A pointer to a pointer at which a buffer containing the bytes of the file are allocated by the function.
///@bug Should probably have an int/enum return that signifies success/failure and reason for failure
void readFile(const FILE_HEADER* header, void** buffer);

///@brief Finds a file header with a specified name
///@param [in]	name	The name of the file to search for.
///@param [in]	header	A pointer to a FILE_HEADER_REG object. This may be NULL to signify a search of the root directory.
FILE_HEADER_REG* findFile(const char* name, const FILE_HEADER* searchLocation);

///@brief Given a regular 8.1 file header, prints out the contents of the file to console.
///@param [in] file	A pointer to a FILE_HEADER_REG.
void cat(const FILE_HEADER_REG* file);

#endif