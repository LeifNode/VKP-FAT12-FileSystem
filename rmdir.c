#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"

#include "pwd.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; rm takes the path of the directory to remove.\n");
		exit(1);
	}
	
	SHELL_SHARED_MEMORY* sharedMem = mapShared();

	openFileSystem(sharedMem->image_path);
	
	readBootSector();

	if (strcmp(argv[1], ".") == 0 || strcmp(argv[1], "..") == 0)
	{
		printf("Cannot use rmdir on %s.\n", argv[1]);
	}
	else
	{
		//FILE_HEADER* selectedFile = (FILE_HEADER*)findFile(argv[1], getDirStackTop(sharedMem));
		
		FILE_HEADER *selectedFile = NULL;
		
		findFile(argv[1], getDirStackTop(sharedMem), &selectedFile);

		if(selectedFile != NULL)
		{
			if ((selectedFile->header.attributes & FILE_ATTR_SUBDIRECTORY) != 0 &&
				selectedFile->header.attributes != 0x0f)//Make sure this file is valid
			{
				//TODO: Support absolute file paths
				
				if (!isDirectoryEmpty(selectedFile))
				{
					printf("Directory still has files.\n");
				}
				else
				{
					if(selectedFile == getDirStackTop(sharedMem))
					{
						char username[33];
						
						getlogin_r(username, 32);
						
						struct passwd *p;
						p = getpwnam(username);
						
						if(p)
						{
							char *first_name = strdup(p->pw_gecos);
							char *first_space = strchr(first_name, ' ');
							
							*first_space = '\0';
							
							printf("Nice try, %s, but deleting the directory you are currently in is not allowed.\n", first_name);
							
							free(first_name);
						}
						else
							printf("Deleting the directory you are currently in is not allowed.\n");
						
						return 0;
					}
					
					freeFatChain(selectedFile->header.first_logical_cluster, true);
					
					memset(selectedFile, 0, sizeof(FILE_HEADER));
					
					collapseDirectory(getDirStackTop(sharedMem));
				}
			}
			else
			{
				printf("Specified file %s is not a directory.\n", getFileNameStringFromFileHeader(selectedFile));
			}
		}
		else
			printf("Directory %s could not be found!\n", argv[1]);
	}

	return 0;
}