/**
 * @file general.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief 
 *
 * @version 0.1
 * @date 2021-02-28
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>



/**
 * @brief 
 *
 * @return char*
 */
char *
gethome( void )
{
    char *home = getenv("HOME");
    if (home == NULL)
    {
        perror("gethome");
        exit(EXIT_FAILURE);
    }

    return home;
}
