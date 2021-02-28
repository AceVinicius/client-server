/**
 * @file main.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief Controls the main flux of the client app
 * @version 0.1
 * @date 2021-02-20
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

#include "../../lib/include/list.h"
#include "../../lib/include/client.h"
#include "../../lib/include/history.h"
#include "../../lib/include/parse.h"
#include "../../lib/include/execute.h"
#include "../../lib/include/allocation.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <pwd.h>
#include <ftw.h>



/**
 * @brief Constants
 */
static char *input = (char *) NULL;



/**
 * @brief Initialize autocomplete in terminal and
 *        get latest history as input for older
 *        entries
 * 
 */
void
initialize_readline( void )
{
    rl_initialize();
    using_history();

    const char *path = get_history_path();

    if (access(path, W_OK) == 0)
    {
        read_history(path);
    }
}



/**
 * @brief Builds the string before the user input
 * 
 * @param prompt The string to save the builded prompt 
 */
void
build_prompt( char *prompt ) 
{
    const char *k_user = getenv("USER");
    const char *k_home = getenv("HOME");
    const char *k_pwd  = getenv("PWD");

    if (k_user == NULL ||
        k_pwd  == NULL ||
        k_home == NULL)
    {
        perror("getenv");
        exit(EXIT_FAILURE);
    }

    char *host = (char *) allocate(HOST_LIMIT, sizeof(char));
    if (gethostname(host, HOST_LIMIT) == -1)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    char *pwd = NULL;
    
    if (strcmp(k_pwd, k_home) == 0)
    {
        pwd = strdup("~");
    }
    else if (strcmp(k_pwd, "/") == 0)
    {
        pwd = strdup("/");
    }
    else
    {
        pwd = strdup(strrchr(k_pwd, '/') + 1);
    }

    snprintf(prompt, PROMPT_LIMIT, "%s@%s %s %c ", k_user, host, pwd, '%');

    free(host);
    free(pwd);
}



/**
 * @brief Get the user input and add to history if
 *        it's not empty.
 * 
 * @param prompt Prompt formatted for print before user
 *               input
 */
void
get_user_input( const char *prompt )
{
    if (input != NULL)
    {
        free(input);
        input = (char *) NULL;
    }

    input = readline(prompt);

    if (input && *input)
    {
        add_history(input);
        add_input_to_history(input);
    }
}



int
main( void )
{
    initialize_readline();
    
    bool status = true;

    do
    {
        char *prompt = (char *) allocate(PROMPT_LIMIT, sizeof(char));
        
        build_prompt(prompt);
        get_user_input(prompt);

        LIST *command = initialize_list();
        
        parse(command, input);
        status = execute(command);
        
        free_command(command);
    }
    while (status);

    return EXIT_SUCCESS;
}
