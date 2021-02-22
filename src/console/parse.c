/**
 * @file parse.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief This function will split the prompt line into tokens.
 *        This tokens will be splitted in three matrix:
 *
 *          commands  -> will store what the user wants to do;
 *          options   -> will store the parameters of the function;
 *          direction -> will store the way I/O will be processed.
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



char ***
parse( char *input )
{
    unsigned int token_size_commands  = TOKEN_LIMIT;
    unsigned int token_size_options   = TOKEN_LIMIT;
    unsigned int position_commands    = 0;
    unsigned int position_options     = 0;

    char ***transfer = (char ***) malloc(sizeof(char **) * 3);
    char  **commands = (char  **) malloc(sizeof(char  *) * token_size_commands);
    char  **options  = (char  **) malloc(sizeof(char  *) * token_size_options);
    char   *token;

    // TODO: Check dynamic allocation

    token = strtok(input, WHITESPACE);

    for (unsigned int i = 0; token[ i ]; ++i)
    {
        token[ i ] = tolower(token[ i ]);
    }

    while (token != NULL)
    {
        if (token[ 0 ] == '-')
        {
            options[ position_options++ ] = token;

            if (position_options >= token_size_options)
            {
                token_size_options += TOKEN_LIMIT;
                options = realloc(options, sizeof(char *) * token_size_options);
                if (options == NULL)
                {
                    // TODO: free all other variables
                    perror("parse");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            commands[ position_commands++ ] = token;

            if (position_commands >= token_size_commands)
            {
                token_size_commands += TOKEN_LIMIT;
                commands = realloc(commands, sizeof(char *) * token_size_commands);
                if (commands == NULL)
                {
                    // TODO: free all other variables
                    perror("parse");
                    exit(EXIT_FAILURE);
                }
            }
        }
        token = strtok(NULL, WHITESPACE);
    }

    commands[ position_commands ]   = NULL;
    options[ position_options ]     = NULL;

    transfer[ 0 ] = commands;
    transfer[ 1 ] = options;

    return transfer;
}
