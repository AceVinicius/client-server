/**
 * @file main.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief 
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

#include "../../lib/include/client.h"
#include "../../lib/include/history.h"
#include "../../lib/include/parse.h"
#include "../../lib/include/execute.h"

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
 * @brief Defines
 */
# define PROMPT_LIMIT 128
# define HOST_LIMIT    32



/**
 * @brief Prototypes
 */
void  initialize_readline  ( void );
void  build_prompt         ( char * );
void  get_user_input       ( char * );
int   main                 ( /*const int, const char ***/ );



/**
 * @brief Constants
 */
static char *input = (char *) NULL;






void
initialize_readline( void )
{
    rl_initialize();
    using_history();

    if (access(get_history_path(), W_OK) == 0)
    {
        read_history(get_history_path());
    }
}



void
build_prompt( char *prompt ) 
{
    char *user = getenv("USER");
    // char *host = getenv("HOST"); // !BUG -> (null) host value when uncommented
    char *pwd  = getenv("PWD");
    char *home = getenv("HOME");
    
    if (user == NULL ||
        // host == NULL ||
        pwd  == NULL ||
        home == NULL)
    {
        perror("getenv");
        exit(EXIT_FAILURE);
    }

    char host[ HOST_LIMIT ];
    if (gethostname(host, HOST_LIMIT) == -1)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    if (strcmp(pwd, home) == 0)
    {
        pwd = "~";
    }
    else if (strcmp(pwd, "/") != 0)
    {
        pwd = strrchr(pwd, '/') + 1;
    }

    snprintf(prompt, PROMPT_LIMIT, "[%s@%s %s] %c ", user, host, pwd, '%');
}



void
get_user_input(char *prompt)
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
main( /*const int   argc    ,
      const char **argv*/ )
{
    initialize_readline();
    
    bool status = true;

    do
    {
        char prompt[ PROMPT_LIMIT ];

        // reset(); style(BOLD, RED);  //Only for differentiating from original terminal
        
        build_prompt(prompt);
        get_user_input(prompt);

        char ***receive = parse(input);
        char  **command = *(receive + 0);
        char  **options = *(receive + 1);

        status = execute(command, options);

        free(receive);
        free(command);
        free(options);
    }
    while (status);

    // reset();

    return EXIT_SUCCESS;
}
