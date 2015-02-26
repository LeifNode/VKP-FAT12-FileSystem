#pragma once
#ifndef _EXECUTABLES_H
#define _EXECUTABLES_H

//A header to deal with an executable file list and a 

//Allocation chunk size for executables list.
#define EXECUTABLES_ALLOC_CHUNK_SIZE 16

const unsigned char ELF_HEADER[] = { 0x7f, 'E', 'L', 'F' };

//Windows
//unsigned char ELF_HEADER[] = { 'M', 'Z' };

char **EXECUTABLES = NULL;

//Stores the number of entry slots allocated in the executable list.
size_t EXECUTABLES_SIZE = 0;

//Stores the actual number of entries populated in the executable list.
size_t NUM_EXECUTABLES = 0;

#define ELF_HEADER_SIZE sizeof(ELF_HEADER)/sizeof(ELF_HEADER[0])

///@brief Determines if a file is a valid executable ELF file.
///@param [in] fp	A FILE pointer to an open file.
///@retval true		The file is a valid ELF.
///@retval false	The file is not executable ELF or the file has not been opened.
bool isELF(FILE *fp);

///@brief Frees the executables list.
void freeExecutableList();

///@brief Adds an executable to the executables list.
///@param [in] name	A null-terminated character string representing an executable's filename.
void addExecutable(char *name);

///@brief Prints a list of all executables.
void printExecutables();

///@brief Trims off unused executable entries.
void trimExecutables();	

///@brief Adds the executables of a directory to the executable list.
///@param [in] dir	A string path to a directory.
void addDirToExecutableList(char *dir);

#endif
