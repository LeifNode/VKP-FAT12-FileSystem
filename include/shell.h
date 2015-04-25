#ifndef SHELL_H
#define SHELL_H

#include "common.h"
#include <sys/wait.h>
#include <unistd.h>

///@brief	Fork off the shell and execute a process, giving it a list of optional arguments.
///@param[in]	path		A C-string holding the path to the executable to be run by the forked off shell.
///@param[in]	arguments	An array of C-string arguments to be passed to the executable to be run by the forked off shell.
void execProcess(const char* path, char* arguments[]);

///@todo Understand this a bit better.
int parseCommand(char* command, char*** commandArr);

///@todo Understand this a bit better.
void parsePathFileExtension(char* fullPath, char** pathOut, char** fileNameOut, char** extensionOut);

///@todo Understand this a bit better.
//void parsePathFile(char* fullPath, char** pathOut, char** fileNameOut);

#endif
