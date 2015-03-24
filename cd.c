#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"
#include "fileio.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; cd takes a file name to search for.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();
	
	if (strcmp(argv[1], ".") == 0)
	{}
	else if (strcmp(argv[1], "..") == 0)
	{
		popDirStack(sharedMem);
		
		FILE_HEADER* top = getDirStackTop(sharedMem);
		
		if (top != NULL)
		{
			printFileHeader(top);
		}
		else
		{
			//printf("root\n");
		}
	}
	else
	{
		if(argv[1][0] = '/')
		{
			//Clear dir stack.
			sharedMem->stack_top_index = 0;
		}
		
		//Copy our path string over, because it is about to get mangled by strtok.
		char *path = malloc(strlen(argv[1]));
		strcpy(path, argv[1]);
	
		//Keep track of what we've pushed onto the stack in case we have to get rid of it.
		//int stackDepth = 0;
		
		//Begin tokenizing
		char *nextPathElement = strtok(path, "/");
		
		while(nextPathElement != NULL)
		{
			FILE_HEADER* selectedDir = (FILE_HEADER*)findFile(nextPathElement, getDirStackTop(sharedMem));
			
			if(selectedDir)
			{
				if((selectedDir->header.attributes & FILE_ATTR_SUBDIRECTORY) != 0)
				{
					printFileHeader(selectedDir);
					
					//Push onto the direcctory stack.
					pushDirStack(sharedMem, selectedDir);
					
				}
				else
				{
					fprintf(stderr, "%s is not a directory!\n", nextPathElement);
				}
			}
			else
			{
				fprintf(stderr,"Unable to find %s!\n", nextPathElement);
				break;
			}
			
			nextPathElement = strtok(NULL, "/");
		}
		
		free(path);
	}

	return 0;
}