/**
 * @file parse.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief This function will split the prompt line into tokens. This tokens
 *        will be splitted in two matrix:
 *
 *          command   -> will store what the user wants to do;
 *          options   -> will store the parameters of the command;
 *          arguments -> will store the files we want to operate
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

#include "../../lib/include/allocation.h"
#include "../../lib/include/list.h"
#include "../../lib/include/parse.h"



/**
 * @brief Tokenize the user input.
 * 
 * @param command A list to store the tokens from user input.
 * @param input User input string.
 */
void
parse( LIST *command ,
       char *input   )
{
    command->command = strtok(input, WHITESPACE);
    
    char *token = strtok(NULL, WHITESPACE);
    
    while (token != NULL)
    {
        if (token[ 0 ] == '-')
        {
            insert_option(command, token);
        }
        else
        {
            insert_argument(command, token);
        }
        
        token = strtok(NULL, WHITESPACE);
    }
    
    return;
}
