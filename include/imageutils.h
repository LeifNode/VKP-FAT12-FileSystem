#include "common.h"

#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#ifdef _WIN32

///@brief Handle for memory map object.
extern HANDLE hMap;

///@brief Handle for file object.
extern HANDLE hImage;

#endif

///@brief Memory map array for file.
extern uint8_t *FILE_SYSTEM;

///@brief Memory maps the file system to FILE_SYSTEM
///@param[in] path	A const character string representing a path to an image file to mount.
///@retval	true	The mount is successful.
///@retval	false	The mount is unsuccessful.
bool openFileSystem(const char* path);

///@brief Closes memory map
void closeFileSystem();

#endif