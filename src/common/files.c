/**
 * @file files.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief Do files operations
 *
 * @version 0.1
 * @date 2021-02-28
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>

#include "../../lib/include/files.h"



/**
 * @brief Opens any kind of file.
 *
 * @param file The name of the file.
 * @param mode The mode which the file will be opened.
 *
 * @return FILE* A pointer to the opened file.
 */
FILE *
open_file( const char *file ,
           const char *mode )
{
    FILE *ptr = fopen(file, mode);
    if (ptr == NULL)
    {
        perror("open_file");
        exit(EXIT_FAILURE);
    }

    return ptr;
}



/**
 * @brief Closes a file pointer.
 *
 * @param ptr The file to be closed.
 */
void
close_file( FILE *ptr )
{
    if (fclose(ptr) == EOF)
    {
        perror("close_file");
        exit(EXIT_FAILURE);
    }

    ptr = NULL;
}
