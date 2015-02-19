#include "common.h"
#include "shell.h"
#include "bootsector.h"
#include "console.h"


int main(int argc, char* argv[])
{	
	while (1)
	{
		char* line = getLine();

		//printf("%s\n", line);
		
		char** command = parseCommand(line);

		if (strcmp(command[0], "exit") == 0)
		{
			free(line);
			free(command);
			break;
		}

		free(line);
		free(command);
	}
	

	return 0;
}
