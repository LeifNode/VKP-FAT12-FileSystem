#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "fileio.h"

///@brief Main function for echo.
///@test Echo shall print the first argument provided to it to console.
///@test If echo is provided with a number of arguments other than one, echo shall exit printing, "Invalid argument count; need something to echo.".
int echo_main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; need something to echo.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();
	
	printf(argv[1]);
	
	putchar('\n');

	return 0;
}

int main(int argc, char* argv[])
{
	return echo_main(argc, argv);
}