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

	int currentIndex = 1;
	
	
	do 
	{
		printf("/");
		
		FILE_HEADER* file = getDirStackIndex(sharedMem, currentIndex);
		
		if (file != NULL)
		{
			for (int i = 0; i < 8; i++)
			{
				if (file->header.file_name[i] != ' ')
				{
					printf("%c", file->header.file_name[i]);
				}
				else
				{
					break;
				}
			}
		}
		
		currentIndex++;
	} while (currentIndex <= sharedMem->stack_top_index);

	printf("\n");
	
	return 0;
}