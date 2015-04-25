#ifndef SHELL_H
#define SHELL_H

#include "common.h"
#include <sys/wait.h>
#include <unistd.h>

///@brief Fork off the shell and execute a process, giving it a list of optional arguments.
///@param[in]	path		A C-string holding the path to the executable to be run by the forked off shell.
///@param[in]	arguments	An array of C-string arguments to be passed to the executable to be run by the forked off shell.
void execProcess(const char* path, char* arguments[]);

///@brief Parses a command from its arguments.
///@param[in] command		The command to parse.
///@param[out] commandArr	The command array output.
///@return Returns the number of arguments delimited by spaces.
int parseCommand(char* command, char*** commandArr);

///@warning Made obsolete by getFileHeaderNameChunkFromFileNameString() in fileio.h.
void parsePathFileExtension(char* fullPath, char** pathOut, char** fileNameOut, char** extensionOut);

///@todo Understand this a bit better.
//void parsePathFile(char* fullPath, char** pathOut, char** fileNameOut);

#endif
