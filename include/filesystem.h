#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "common.h"

extern uint8_t *FILE_SYSTEM;

void openFileSystem(const char* path);
void closeFileSystem();

#endif