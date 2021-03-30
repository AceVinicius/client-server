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
#include <string.h>
#include <stdlib.h>

#include "../../lib/include/allocation.h"
#include "../../lib/include/general.h"



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



char *
strdup( const char *input )
{
    if (input == NULL)
    {
        return NULL;
    }

    int len = strlen(input);

    char *output = (char *) allocate(len, sizeof(char));
    output = (char *) memcpy(output, input, len);

    return output;
}



/**
 * @brief Campare two words to see if they are equal
 *
 * @param str1 Word 1
 * @param str2 Word 2
 * @return int Returns 1 if they are equal and 0 if not
 */
int
cmp( const char *str1 ,
     const char *str2 )
{
    return !strcmp(str1, str2);
}
