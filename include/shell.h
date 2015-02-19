#ifndef SHELL_H
#define SHELL_H

#include "common.h"
#include <sys/wait.h>
#include <unistd.h>

void execProcess(const char* path, char* arguments[]);

char** parseCommand(char* command);

#endif
