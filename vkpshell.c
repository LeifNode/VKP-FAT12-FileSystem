#include "common.h"
#include "shell.h"
#include "bootsector.h"
#include "console.h"
#include "sharedmemory.h"


int main(int argc, char* argv[])
{	
	createShared();
	SHELL_SHARED_MEMORY* shellShared = mapShared();
	
	shellShared->current_dir_flc = 0;
	shellShared->file_system = NULL;

	while (1)
	{
		printf("shell:");
	
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
			else if (strcmp(command[0], "pbs") == 0)
			{
				char* fileName = "./pbs.exe";
				command[0] = fileName;
				execProcess("./bin/pbs.exe", command);
			}
			else if (strcmp(command[0], "pfe") == 0)
			{
				char* fileName = "./pfe.exe";
				command[0] = fileName;
				execProcess("./bin/pfe.exe", command);
			}
			else if (strcmp(command[0], "mount") == 0)
			{
				
			}
			else
			{
				printf("%s: command not found.\n", command[0]);
			}
		}

		free(line);
		free(command);
	}
	
	unmapShared();
	
	return 0;
}
