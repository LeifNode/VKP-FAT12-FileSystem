#include "fileio.h"

void printFileHeader(const FILE_HEADER* header)
{
    printf("File name               = ");
    
    int i = 0;
    for(i = 0; i < 8; i++)
    {
        printf("%c", header->file_name[i]);
    }

    printf("\nExtension               = ");
    
    for(i = 0; i < 8; i++)
    {
        printf("%c", header->extension[i]);
    }
    
    printf("\nAttributes              = %x\n", header->attributes);
    printf("First logical cluster   = %d\n", header->first_logical_cluster);
    printf("File size               = %dB\n", header->file_size); 
}
