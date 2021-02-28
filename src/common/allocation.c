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
    void *new_ptr = calloc((size + 1), type);
    if (new_ptr == NULL)
    {
        perror("allocate");
        exit(EXIT_FAILURE);
    }

    return new_ptr;
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
        exit(EXIT_FAILURE);
    }

    void *new_ptr = realloc(ptr, (new_size + 1) * type);
    if (new_ptr == NULL)
    {
        perror("reallocate");
        exit(EXIT_FAILURE);
    }

    return new_ptr;
}
