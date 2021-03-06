#include "global_limits.h"

#include "common.h"
#include "shell.h"
#include "bootsector.h"
#include "console.h"
#include "sharedmemory.h"
#include "fileio.h"
#include "imageutils.h"
#include "executables.h"

#include "stdbool.h"

#define SHELL_PROMPT "shell~"

///@brief Display a list of shell commands available.
void showHelp()
{
	printf("Shell commands available:\n----------------------\n\n");
	printf("Type 'help' to view this list again.\n\n");
	printExecutables();
}

int main(int argc, char* argv[])
{	
	//Free any mappings from earlier
	unmapShared();
	
	//Create shared memory map.
	createShared();
	
	//Map it using mmap() to shellShared.
	SHELL_SHARED_MEMORY* shellShared = mapShared();
	
	if(shellShared == NULL)
	{
		fprintf(stderr, "Shared memory could not be mapped!\n");
		
		exit(EXIT_FAILURE);
	}
	
	memset(shellShared, 0, sizeof(shellShared));
	
	//Add bin directory to executable list.
	
	addDirToExecutableList("./bin");
	
	printf("\n\nWelcome to VKP Shell.\n\n");
	
	showHelp();
	
	putchar('\n');

	while (1)
	{
		openFileSystem(shellShared->image_path);
	
		printf(SHELL_PROMPT);
		//printWorkingDirectory(shellShared);
		printf(getWorkingPathFromStack(shellShared));
		printf(": ");
	
		char* line = getLine();
		char** command;
		int argcount = parseCommand(line, &command);
		
		if (argcount > 0)
		{
			if (strcmp(command[0], "exit") == 0)
			{
				free(line);
				free(command);
				break;
			}
			else if (strcmp(command[0], "help") == 0)
			{
				showHelp();
				putchar('\n');
			}
			/*else if (strcmp(command[0], "pbs") == 0)
			{
				char* fileName = "./pbs";
				command[0] = fileName;
				execProcess("./bin/pbs", command);
			}
			else if (strcmp(command[0], "pfe") == 0)
			{
				char* fileName = "./pfe";
				command[0] = fileName;
				execProcess("./bin/pfe", command);
			}
			else if (strcmp(command[0], "mount") == 0)
			{
				char* fileName = "./mount";
				command[0] = fileName;
				execProcess("./bin/mount", command);
			}
			else if (strcmp(command[0], "cd") == 0)
			{
				char* fileName = "./cd";
				command[0] = fileName;
				execProcess("./bin/cd", command);
			}
			else if (strcmp(command[0], "cat") == 0)
			{
				char* fileName = "./cat";
				command[0] = fileName;
				execProcess("./bin/cat", command);
			}
			else if (strcmp(command[0], "rm") == 0)
			{
				char* fileName = "./rm";
				command[0] = fileName;
				execProcess("./bin/rm", command);
			}*/
			else
			{
				bool found = false;
				for(size_t i = 0; i < NUM_EXECUTABLES; ++i)
				{
					char *executable_name = strrchr(EXECUTABLES[i], '/') + 1;
					
					//If what the user entered was a valid filename, then execute it.
					if(strcmp(executable_name, command[0]) == 0)
					{
						execProcess(EXECUTABLES[i], command);
						found = true;
						break;
					}
				}
				
				if(!found)
					printf("%s: command not found.\n", command[0]);
			}
		}

		free(line);
		free(command);
	}
	
	//Free executables list.
	freeExecutableList();
	
	unmapShared();
	
	return 0;
}
