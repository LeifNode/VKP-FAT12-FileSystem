#include "shell.h"
#include "console.h"


int main(int argc, char* argv[])
{
    while (1)
    {
        
        char* line = getLine();

	printf(line);
	
	char key[] = "exit\n";

	if (strcmp(line, key) == 0)
	{
	    free(line);
	    break;
	}

	free(line);
    }

    printf("Exiting..\n");

    return 0;
}
