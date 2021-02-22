/**
 * @file parse.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief This function will split the prompt line into tokens.
 *        This tokens will be splitted in two matrix:
 *
 *          commands  -> will store what the user wants to do;
 *          options   -> will store the parameters of the function;
 * 
 * @version 0.1
 * @date 2021-02-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../lib/include/parse.h"
#include "../../lib/include/allocation.h"



char ***
parse( char *input )
{
    size_t position_commands = 0;
    size_t position_options  = 0;

    char ***transfer = (char ***) allocate(2, sizeof(char **));
    char  **commands = (char  **) allocate(1, sizeof(char  *));
    char  **options  = (char  **) allocate(1, sizeof(char  *));

    char *token = strtok(input, WHITESPACE);

    for (size_t i = 0; token[ i ]; ++i)
    {
        token[ i ] = tolower(token[ i ]);
    }

    while (token != NULL)
    {
        if (token[ 0 ] == '-')
        {
            options = (char  **) reallocate(options, position_options+1, sizeof(char *));
            options[ position_options++ ] = token;
        }
        else
        {
            commands = (char  **) reallocate(commands, position_commands+1, sizeof(char *));
            commands[ position_commands++ ] = token;
        }
        
        token = strtok(NULL, WHITESPACE);
    }

    /* // TODO: Check if can be removed
    commands[ position_commands ] = NULL;
    options[ position_options ]   = NULL; */

    transfer[ 0 ] = commands;
    transfer[ 1 ] = options;

    return transfer;
}
