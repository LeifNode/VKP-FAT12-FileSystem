#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "sectors.h"


///@brief Main function for pwd command.
///@test If the pwd command is invoked with any number of arguments, the current working path of the shell shall be displayed.
int pwd_main(int argc, char* argv[])
{	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	printf("%s\n",getWorkingPathFromStack(sharedMem));
	
	return 0;
}

int main(int argc, char* argv[])
{
	return pwd_main(argc, argv);
}