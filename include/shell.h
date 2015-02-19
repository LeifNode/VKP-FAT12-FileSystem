#include "common.h"
#include <sys/wait.h>
#include <unistd.h>

#ifndef SHELL_H
#define SHELL_H

void execProcess(const char* path, char* arguments[]);

char** parseCommand(char* command);

#endif
