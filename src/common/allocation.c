/**
 * @file allocation.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief Do dynamic memory aperations with ease around all project.
 *
 * @version 0.1
 * @date 2021-02-22
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/include/allocation.h"



/**
 * @brief Allocate dynamic memory with calloc and check pointer.
 *
 * @param size Quantity of blocks desired.
 * @param type The size of each block.
 * @return void* Returns the address of the allocated block.
 */
void *
allocate( const size_t size ,
          const size_t type )
{
    void *new_ptr = (void *) calloc((size + 1), type);
    if (new_ptr == NULL)
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    return (void *) new_ptr;
}



/**
 * @brief Reallocate a dynamic block previously allocated.
 * 
 * @param ptr Old block of memory.
 * @param new_size New quantity of blocks.
 * @param type Size of each block.
 * @return void* Returns the address of the new allocated block.
 */
void *
reallocate(       void   *ptr      ,
            const size_t  new_size ,
            const size_t  type     )
{
    if (ptr == NULL)
    {
        fprintf(stderr, "reallocate: NULL Pointer Given: ptr\n");
        exit(EXIT_FAILURE);
        allocate(new_size, type);
    }

    void *new_ptr = (void *) realloc(ptr, (new_size + 1) * type);
    if (new_ptr == NULL)
    {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    return (void *) new_ptr;
}



char *
duplicate( const char *input )
{
    if (input == NULL)
    {
        fprintf(stderr, "duplicate: NULL Pointer Given: input\n");
        exit(EXIT_FAILURE);
    }
    
    char *output = strndup(input, strlen(input));
    if (output == NULL)
    {
        perror("strndup");
        exit(EXIT_FAILURE);
    }

    return output;
}



char *
duplicate_n( const char   *input , 
             const size_t  size  )
{
    if (input == NULL)
    {
        fprintf(stderr, "duplicate: NULL Pointer Given: input\n");
        exit(EXIT_FAILURE);
    }
    
    char *output = strndup(input, size);
    if (output == NULL)
    {
        perror("strndup");
        exit(EXIT_FAILURE);
    }

    return output;
}



void
free_mem( void *ptr )
{
    if (ptr == NULL)
    {
        fprintf(stderr, "free_mem: NULL Pointer Given: ptr\n");
        exit(EXIT_FAILURE);
    }

    free(ptr);
    ptr = NULL;
}
