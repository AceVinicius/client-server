/**
 * @file list.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief Do operations with the LIST.
 *
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



static void  free_option    ( OPTION * );
static void  free_argument  ( ARGUMENT * );



/**
 * @brief Allocate and initialize the list.
 *
 * @return LIST* Returns the address of the created list.
 */
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



/**
 * @brief Insert a node in option head.
 *
 * @param command The address of the list.
 * @param token The token to be inserted in the list.
 */
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



/**
 * @brief Insert a node in argument head.
 *
 * @param command The address of the list.
 * @param token The token to be inserted in the list.
 */
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



/**
 * @brief Frees the entire list.
 *
 * @param command Address of the list.
 */
void
free_command( LIST *command )
{
    if (command == NULL)
    {
        return;
    }

    free_option(command->options);
    free_argument(command->arguments);
    free_mem(command);

    return;
}



/**
 * @brief Free option nodes.
 *
 * @param options Node to be freed.
 */
static void
free_option( OPTION *options )
{
    if (options == NULL)
    {
        return;
    }

    free_option(options->next);
    free_mem(options);

    return;
}



/**
 * @brief Free argument nodes.
 * 
 * @param arguments  Node to be freed.
 */
static void
free_argument( ARGUMENT *arguments )
{
    if (arguments == NULL)
    {
        return;
    }

    free_argument(arguments->next);
    free_mem(arguments);

    return;
}
