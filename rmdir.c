#include "common.h"
#include "imageutils.h"
#include "bootsector.h"
#include "fat.h"
#include "sharedmemory.h"

#include "pwd.h"


///@brief Main function for rmdir command.
///@test If rmdir is provided with a single argument that is a valid path to a directory, rmdir shall remove that folder from the mounted image.
///@test If rmdir is provided with a single argument that is a valid path to a directory containing any file and/or directory, rmdir shall exit printing the message, "Directory still has files.".
///@test If rmdir is provided with a single argument that is a valid path to something other than a subdirectory, or a long file header, rmdir shall exit printing the message, "Specified file [file_name] is not a directory.".
///@test If rmdir is provided with a single argument that is an invalid path rmdir shall exit printing the message, "Directory [path] could not be found!".
///@test If rmdir is provided with a number of arguments other than one, rmdir shall exit printing, "Invalid argument count; rmdir takes the path of the directory to remove.".
///@test If rmdir is directed to delete the current working directory, the working directory has no files within it, and the user name of the current user can be obtained, rmdir shall exit printing "Nice try [user_name], but deleting the directory you are currently in is not allowed.".
///@test If rmdir is directed to delete the current working directory and the working directory has no files within it, rmdir shall exit printing, "Deleting the directory you are currently in is not allowed.".
///@test If rmdir successfully deletes a directory, rmdir shall attempt to collapse the parent directory deleted from.

int rmdir_main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Invalid argument count; rmdir takes the path of the directory to remove.\n");
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

int main(int argc, char* argv[])
{
	return rmdir_main(argc, argv);
}