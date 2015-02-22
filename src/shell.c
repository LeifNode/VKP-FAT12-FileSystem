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

int parseCommand(char* command, char*** commandArr)
{
	int capacity = 10;
    char** commandArrTmp = (char**) malloc(sizeof(char*) * capacity);
	
	int i = 0;
	
	char* token;
	token = strtok(command, " ");
	
	while (token != NULL)
	{
		commandArrTmp[i++] = token;
		
		if (capacity <= i)
		{
			capacity *= 2;
			commandArrTmp = realloc(commandArrTmp, capacity);
		}
		
		token = strtok(NULL, " ");
	}
	
	commandArrTmp[i] = NULL;
	
	if (commandArr)
		*commandArr = commandArrTmp;
	else
		free(commandArrTmp);
	
	return i;
}
