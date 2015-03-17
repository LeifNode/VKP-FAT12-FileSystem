#pragma once
#ifndef _EXECUTABLES_H
#define _EXECUTABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>

//A header to deal with an executable file list and a 

//Allocation chunk size for executables list.
#define EXECUTABLES_ALLOC_CHUNK_SIZE 16

//#define ELF_HEADER_SIZE sizeof(ELF_HEADER_BYTES)/sizeof(ELF_HEADER_BYTES[0])
#define ELF_HEADER_SIZE 4

extern const unsigned char ELF_HEADER_BYTES[ELF_HEADER_SIZE];

//Windows
//unsigned char ELF_HEADER[] = { 'M', 'Z' };

extern char **EXECUTABLES;

//Stores the number of entry slots allocated in the executable list.
extern size_t EXECUTABLES_SIZE;

//Stores the actual number of entries populated in the executable list.
extern size_t NUM_EXECUTABLES;

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
