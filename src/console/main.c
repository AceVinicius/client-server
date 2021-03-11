/**
 * @file main.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 *
 * @brief Controls the main flux of the client app.
 *
 * @version 0.1
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "../../lib/include/list.h"
#include "../../lib/include/parse.h"
#include "../../lib/include/client.h"
#include "../../lib/include/general.h"
#include "../../lib/include/history.h"
#include "../../lib/include/execute.h"
#include "../../lib/include/allocation.h"



/**
 * @brief User input
 */
static char *input = (char *) NULL;



/**
 * @brief Initialize autocomplete in terminal and get latest history as input
 *        for older entries.
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
 * @brief Builds the string before the user input.
 *
 * @param prompt The string to save the builded prompt.
 */
void
build_prompt( char *prompt ) 
{
    char *user = (char *) allocate(USER_LIMIT, sizeof(char));
    char *host = (char *) allocate(HOST_LIMIT, sizeof(char));
    char *cwd  = (char *) allocate(CWD_LIMIT , sizeof(char));

    if (getlogin_r(user, USER_LIMIT) == 0)
    {
        perror("getlogin");
        exit(EXIT_FAILURE);
    }

    if (gethostname(host, HOST_LIMIT) == -1)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }
    else
    {
        host = strtok(host, ".");
    }

    if (getcwd(cwd, CWD_LIMIT) == NULL)
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(cwd, gethome()) == 0)
        {
            cwd = strdup("~");
        }
        else if (strcmp(cwd, "/") == 0)
        {
            cwd = strdup("/");
        }
        else
        {
            cwd = strdup(strrchr(cwd, '/') + 1);
        }
    }

    snprintf(prompt, PROMPT_LIMIT, "%s@%s %s %c ", user, host, cwd, '%');

    free_mem(cwd);
    free_mem(host);
    free_mem(user);
}



/**
 * @brief Get the user input and add to history if it's not empty.
 *
 * @param prompt Prompt formatted for print before user input.
 */
void
get_user_input( const char *prompt )
{
    if (input != NULL)
    {
        free_mem(input);
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
