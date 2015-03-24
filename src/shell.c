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

void parsePathFileExtension(char* fullPath, char** pathOut, char** fileNameOut, char** extensionOut)
{
	char* currentChar = fullPath;
	
	char* fileName = NULL;
	char* extension = NULL;
	char* clearChar = NULL;
	
	while(*currentChar != '\0')
	{
		if (*currentChar == '/' || *currentChar == '\\')
		{
			fileName = currentChar + 1;
			clearChar = currentChar;
		}
		
		currentChar++;
	}
	
	if (clearChar)
		*clearChar = '\0';
		
	if (fileName)
	{
		currentChar = fileName;
	}
	else
	{
		currentChar = fullPath;
	}
	
	clearChar = NULL;
	
	while(*currentChar != '\0')
	{
		if (*currentChar == '.')
		{
			extension = currentChar + 1;
			clearChar = currentChar;
		}
		
		currentChar++;
	}
	
	if (clearChar)
		*clearChar = '\0';
	
	if (pathOut)
		*pathOut = fullPath;
	if (fileNameOut)
		*fileNameOut = fileName;
	if (extensionOut)
		*extensionOut = extension;
		
	if (fileName == NULL)
	{
		*pathOut = NULL;
		*fileNameOut = fullPath;
	}
}