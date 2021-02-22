/**
 * @file history.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief 
 * @version 0.1
 * @date 2021-02-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../lib/include/history.h"



/**
 * @brief Constants
 */
const char *k_history_file_name = "/.history";



/**
 * @brief 
 * 
 * @param file_path 
 * @return FILE* 
 */
FILE *
open_file( const char *file_name, const char *mode )
{
    FILE *file_ptr = fopen(file_name, mode);
    if (file_ptr == NULL)
    {
        perror("open_file");
        exit(EXIT_FAILURE);
    }

    return file_ptr;
}



/**
 * @brief 
 * 
 * @param file_ptr 
 */
void
close_file( FILE *file_ptr )
{
    if (fclose( file_ptr ) == EOF)
    {
        perror("close_file");
        exit(EXIT_FAILURE);
    }
}



/**
 * @brief 
 * 
 * @param input 
 */
void
add_input_to_history( char *input )
{
    FILE *history_ptr = open_file(get_history_path(), "a");

    fputs(strcat(input, "\n"), history_ptr);
    if (ferror(history_ptr))
    {
        printf("add_input_to_history failed: %s was not added", input);
    }

    close_file(history_ptr);
}



/**
 * @brief Get the history path object
 * 
 * @return char* 
 */
char *
get_history_path( void )
{
    char *history_path = getenv("HOME");
    if (history_path == NULL)
    {
        perror("get_history_path");
        exit(EXIT_FAILURE);
    }

    if (strstr(history_path, k_history_file_name) == NULL)
    {
        strcat(history_path, k_history_file_name);
    }

    return history_path;
}



/**
 * @brief 
 * 
 * @param command 
 * @param options 
 * @return int 
 */
int
history( void )
{
    FILE *history_ptr = open_file(get_history_path(), "r");

    char history[ HISTORY_LIMIT ];
    
    for (unsigned int i = 1; !feof(history_ptr); i++)
    {
        if (fgets(history, HISTORY_LIMIT, history_ptr) != NULL)
        {
            printf("  %*d  %s", 3, i, history);
        }
        else
        {
            if (ferror(history_ptr))
            {
                printf("history: reading error at line %d", i);
                return EXIT_FAILURE;
            }
        }
    }
    
    close_file(history_ptr);

    return EXIT_SUCCESS;
}
