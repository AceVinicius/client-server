/**
 * @file history.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief
 * @version 0.1
 * @date 2021-02-25
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>

#include "../../lib/include/allocation.h"
#include "../../lib/include/list.h"



static void    free_option      ( OPTION * );
static void    free_argument    ( ARGUMENT * );



LIST *
initialize_list( void )
{
    LIST *new_list = (LIST *) allocate(1, sizeof(LIST));
    
    new_list->command       = NULL;
    new_list->options       = NULL;
    new_list->arguments     = NULL;
    new_list->qtd_options   = 0;
    new_list->qtd_arguments = 0;
    
    return new_list;
}



void
insert_option( LIST *command ,
               char *token   )
{
    if (command == NULL)
    {
        return;
    }

    OPTION *new_option = (OPTION *) allocate(1, sizeof(OPTION));

    new_option->option = token;
    new_option->next = command->options;

    command->options = new_option;
    ++command->qtd_options;

    return;
}



void
insert_argument( LIST *command ,
                 char *token   )
{
    if (command == NULL)
    {
        return;
    }
    
    ARGUMENT *new_argument = (ARGUMENT *) allocate(1, sizeof(ARGUMENT));
    
    new_argument->argument = token;
    new_argument->next = command->arguments;
    
    command->arguments = new_argument;
    ++command->qtd_arguments;
    
    return;
}


void
free_command( LIST *command )
{
    if (command == NULL)
    {
        return;
    }
    
    free_option(command->options);
    free_argument(command->arguments);
    free(command);
    
    return;
}



static void
free_option( OPTION *options )
{
    if (options == NULL)
    {
        return;
    }
    
    free_option(options->next);
    free(options);
    
    return;
}



static void
free_argument( ARGUMENT *arguments )
{
    if (arguments == NULL)
    {
        return;
    }
    
    free_argument(arguments->next);
    free(arguments);
    
    return;
}
