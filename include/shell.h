#ifndef SHELL_H
#define SHELL_H

#include "common.h"
#include <sys/wait.h>
#include <unistd.h>

void execProcess(const char* path, char* arguments[]);

int parseCommand(char* command, char*** commandArr);

void parsePathFileExtension(char* fullPath, char** pathOut, char** fileNameOut, char** extensionOut);

#endif
