#include "shell.h"

void execProcess(const char* path, char* arguments[])
{
    int childStatus;
    int childPID;

    childPID = fork();

    if (childPID == 0)
    {
        if (execv(path, arguments) == -1)
	{
	    printf("Failed to execute process %s\n", path);
	}

	exit(0);
    }
    else
    {
        wait(&childStatus);
    }
}

char** parseCommand(char* command)
{
	int capacity = 10;
    char** commandArr = (char**) malloc(capacity);
	
	int i = 0;
	
	char* token;
	token = strtok(command, " ");
	
	while (token != NULL)
	{
		commandArr[i++] = token;
		
		if (capacity <= i)
		{
			capacity *= 2;
			commandArr = realloc(commandArr, capacity);
		}
		
		token = strtok(NULL, " ");
	}
	
	commandArr[i] = NULL;
	
	return commandArr;
}
