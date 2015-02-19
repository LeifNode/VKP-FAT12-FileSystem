#include "common.h"
#include "shell.h"
#include "bootsector.h"
#include "console.h"
#include "sharedmemory.h"


int main(int argc, char* argv[])
{	
	SHELL_SHARED_MEMORY* shellShared = mapShared();
	
	shellShared->current_dir_flc = 0;

	while (1)
	{
		printf("shell:");
	
		char* line = getLine();
		char** command = parseCommand(line);

		if (command[0] != NULL && strcmp(command[0], "exit") == 0)
		{
			free(line);
			free(command);
			break;
		}
		else if (command[0] != NULL && strcmp(command[0], "pbs") == 0)
		{
			char* args[] = {"./pbs.exe", command[1], NULL};
			execProcess("./bin/pbs.exe", args);
		}

		free(line);
		free(command);
	}
	
	
	
	return 0;
}
