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
	memset(&shellShared->boot_sector, 0, sizeof(BOOT_SECTOR));
	memset(&shellShared->image_path, 0, 256);

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