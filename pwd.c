#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "sectors.h"

int main(int argc, char* argv[])
{	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	printf("%s\n",getWorkingPathFromStack(sharedMem));
	
	return 0;
}