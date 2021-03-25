/**
 * @file cd.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief Change current working directory.
 *
 * @version 0.1
 * @date 2021-02-28
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../lib/include/list.h"
#include "../../lib/include/cd.h"



/**
 * @brief Changes current working directory.
 * 
 * @param command Directory to be changed.
 * @return int Returns 1 if success.
 */
int
cd( const LIST *command )
{
    if (command == NULL)
    {
        fprintf(stderr, "cd");
        exit(EXIT_FAILURE);
    }

    char *directory = (
        command->qtd_arguments == 0 ?
            getenv("HOME") : command->arguments->argument
    );

    if (chdir(directory))
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", directory);
    }

    return EXIT_SUCCESS;
}
