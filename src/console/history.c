/**
 * @file history.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * 
 * @brief Do operations with the history file.
 * 
 * @version 0.1
 * @date 2021-02-21
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/include/history.h"
#include "../../lib/include/allocation.h"
#include "../../lib/include/files.h"



/**
 * @brief Constants
 */
const char k_history_name[ ] = "/.history";
const int k_history_size = sizeof(k_history_name)/sizeof(char);



/**
 * @brief Print a string to the history file.
 * 
 * @param input The string to be printed in the file.
 */
void
add_input_to_history( const char *input )
{
    FILE *history = open_file(get_history_path(), "a");

    fputs(input, history);
    if (ferror(history))
    {
        printf("add_input_to_history failed: %s was not added\n", input);
    }
    fputs("\n", history);
    if (ferror(history))
    {
        printf("add_input_to_history failed: \\n was not added\n");
    }

    close_file(history);
}



/**
 * @brief Get the full path of the history.
 * 
 * @return char* Returns a string with the full path of the history file.
 */
char *
get_history_path( void )
{
    const char *home = getenv("HOME");
    size_t size = strlen(home);
    char *path = (char *) allocate(size, sizeof(char));
    
    if (strncpy(path, home, size) == NULL)
    {
        perror("get_history_path");
        exit(EXIT_FAILURE);
    }

    if (strstr(path, k_history_name) == NULL)
    {
        size_t new_size = size + k_history_size;
        path = (char *) reallocate(path, new_size, sizeof(char));

        if (strncat(path, k_history_name, new_size) == NULL)
        {
            perror("get_history_path");
            exit(EXIT_FAILURE);
        }
    }

    return path;
}



/**
 * @brief Prints all the information in the history file, formatted.
 * 
 * @return int Returns 0 if success or 1 if failure.
 */
int
history( void )
{
    FILE *history = open_file(get_history_path(), "r");

    char buffer[ HISTORY_LIMIT ];
    
    for (size_t i = 1; !feof(history); i++)
    {
        if (fgets(buffer, HISTORY_LIMIT, history) != NULL)
        {
            printf("  %*zu  %s", 3, i, buffer);
        }
        else
        {
            if (ferror(history))
            {
                printf("history: reading error at line %zu", i);
                return EXIT_FAILURE;
            }
        }
    }
    
    close_file(history);

    return EXIT_SUCCESS;
}
