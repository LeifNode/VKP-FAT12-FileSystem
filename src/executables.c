#include "executables.h"
#include "global_limits.h"

const unsigned char ELF_HEADER_BYTES[ELF_HEADER_SIZE] = { 0x7f, 'E', 'L', 'F' };

char **EXECUTABLES = NULL;

//Stores the number of entry slots allocated in the executable list.
size_t EXECUTABLES_SIZE = 0;

//Stores the actual number of entries populated in the executable list.
size_t NUM_EXECUTABLES = 0;

///@brief Determines if a file is a valid executable ELF file.
///@param [in] fp	A FILE pointer to an open file.
///@retval true		The file is a valid ELF.
///@retval false	The file is not executable ELF or the file has not been opened.
bool isELF(FILE *fp)
{
	fprintf(stderr,"Checking file %p...\n", fp);
	if(!fp)
		return false;

	//Set cursor to beginning of file.
	fseek(fp, 0, SEEK_SET);
	
	//Create a buffer to store the header in and zero it.
	unsigned char header[ELF_HEADER_SIZE] = {0};

	//Read into the buffer.  If the file is less than 4 bytes, fread will just return the number of bytes read.
	//We don't care because we'll find out when we do our memcmp with the ELF_HEADER_BYTES.
	fread(header, ELF_HEADER_SIZE, 1, fp);
	
	//Check this against the expected ELF_HEADER.
	return memcmp(header, ELF_HEADER_BYTES, ELF_HEADER_SIZE) == 0;
}

///@brief Frees the executables list.
void freeExecutableList()
{
	for(size_t i = 0; i < EXECUTABLES_SIZE; ++i)
	{
		free(EXECUTABLES[i]);
	}

	free(EXECUTABLES);
}

///@brief Adds an executable to the executables list.
///@param [in] name	A null-terminated character string representing an executable's filename.
void addExecutable(char *name)
{
	fprintf(stderr,"Adding executable: %s\n", name);
	if(!EXECUTABLES)
	{
		EXECUTABLES_SIZE = EXECUTABLES_ALLOC_CHUNK_SIZE;
		EXECUTABLES = malloc(sizeof(char*) * EXECUTABLES_SIZE);
	}
	 NUM_EXECUTABLES++;

	 if(NUM_EXECUTABLES > EXECUTABLES_SIZE)
	 {
		 EXECUTABLES_SIZE += EXECUTABLES_ALLOC_CHUNK_SIZE;

		 realloc(EXECUTABLES, sizeof(char*) * EXECUTABLES_SIZE);
	 }

	 //Allocate new string.
	 char *newString = malloc(sizeof(char) * strlen(name));

     strcpy(newString, name);

	 EXECUTABLES[NUM_EXECUTABLES-1] = newString;
}

///@brief Prints a list of all executables.
void printExecutables()
{
	for(int i = 0; i < NUM_EXECUTABLES; ++i)
	{
        printf("%s\n", EXECUTABLES[i]);
	}
}

///@brief Trims off unused executable entries.
void trimExecutables()
{
	//Ensure executables array is allocated.
	if(EXECUTABLES)
	{
		realloc(EXECUTABLES, sizeof(char*) * NUM_EXECUTABLES);
		EXECUTABLES_SIZE = NUM_EXECUTABLES;
	}
}

///@brief Adds the executables of a directory to the executable list.
///@param [in] dir	A string path to a directory.
void addDirToExecutableList(char *indir)
{
    struct dirent *dir;

    DIR *d = opendir(indir);

    if(d)
    {
        while((dir = readdir(d)) != NULL)
        {
            char path[MAX_PATH_SIZE];
			
			if(!indir || strcmp(indir,"") == 0)
				strcpy(path, dir->d_name);
			else
				sprintf(path, "%s/%s", indir, dir->d_name);
            //printf("%s\n",path);
            //addExecutable(dir->d_name);

            FILE *fp = fopen(path, "rb");

			if(fp)
			{
				if(isELF(fp))
				{
					//printf("%s\n",dir->d_name);
					addExecutable(path);
				}

				fclose(fp);
			}
        }

		//Slim down the list
		trimExecutables();
    }

    //printExecutables();
}
