#include "console.h"

char* getLine()
{
    char* line = malloc(50);
    char* linep = line;
    size_t lineLengthMax = 50, lineLength = lineLengthMax;
    char c;

    if (line == NULL)
        return NULL;

    while(1)
    {
        c = fgetc(stdin);
	
		if (c == EOF)
			break;
		
		if (--lineLength == 0)
		{
			lineLength = lineLengthMax;
			char* newLine = realloc(linep, lineLengthMax *= 2);

			if (newLine == NULL)
			{
				free(linep);
				return NULL;
			}

			line = newLine + (line - linep);
			linep = newLine;
		}
		
		if((*line++ = c) == '\n')
			break;
    }
	
    *(line - 1) = '\0';
    return linep;
}

void parseCommandArgs(char* str, char* out[])
{
    if (str == NULL || out == NULL)
    {
		printf("Could not parse command arguments.\n");
    }
}
