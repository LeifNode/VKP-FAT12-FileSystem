#include "common.h"

#ifdef _WIN32

//Handle for memory map object.
extern HANDLE hMap;

//Handle for file object.
extern HANDLE hImage;

#endif

//Memory map array for file.
extern uint8_t *FILE_SYSTEM;

//Memory maps the file system to FILE_SYSTEM
bool openFileSystem(const char* path);

//Closes memory map
void closeFileSystem();
