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
#include <dirent.h>
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
#include "../../lib/include/sockets.h"
#include "../../lib/include/history.h"
#include "../../lib/include/execute.h"
#include "../../lib/include/allocation.h"
#include "../../lib/include/directory.h"






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
    using_history();

    char *path = get_history_path();

    if (access(path, W_OK) == 0)
    {
        read_history(path);
    }

    free_mem(path);
}



/**
 * @brief Builds the string before the user input.
 *
 * @param prompt The string to save the builded prompt.
 */
void
build_prompt( char *prompt ) 
{
    if (prompt == NULL)
    {
        fprintf(stderr, "build_prompt: NULL Pointer Given: prompt\n");
        exit(EXIT_FAILURE);
    }

    char *host = (char *) allocate(HOST_LIMIT, sizeof(char));
    char *cwd  = (char *) allocate(CWD_LIMIT , sizeof(char));

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
        if (cmp(cwd, getenv("HOME")))
        {
            free_mem(cwd);
            cwd = strndup("~", CWD_LIMIT);
        }
        else if (cmp(cwd, "/"))
        {
            free_mem(cwd);
            cwd = strndup("/", CWD_LIMIT);
        }
        else
        {
            char *tmp = cwd;
            cwd = strndup(strrchr(cwd, '/') + 1, CWD_LIMIT);
            free_mem(tmp);
        }
    }

    snprintf(prompt, PROMPT_LIMIT, "[%s@%s %s] %c ", getenv("USER"), host, cwd, '%');

    free_mem(cwd);
    free_mem(host);
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
        // free_mem(input);
        input = (char *) NULL;
    }

    input = readline(prompt);

    if (input && *input)
    {
        add_history(input);
        add_input_to_history(input);
    }
}



void
send_files_to_server( const char *folder)
{
    if (folder == NULL)
    {
        fprintf(stderr, "send_files_to_server: NULL Pointer Given: folder\n");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server;
    int server_fd = socket_client(&server);

    send_int(server_fd, 0);

    struct dirent *file = NULL;
    DIR *directory = open_dir(folder);
    
    int num_files;
    for (num_files = -2; (file = readdir(directory)) != NULL; ++num_files);

    directory = reopen_dir(directory, folder);

    send_int(server_fd, num_files);
    while ((file = readdir(directory)) != NULL)
    {
        if (cmp(file->d_name, ".") || cmp(file->d_name, ".."))
        {
            continue;
        }

        send_str(server_fd, file->d_name);
    }

    socket_close(server_fd);
    close_dir(directory);

    return;
}



void
remove_files_from_server( const char *folder )
{
    if (folder == NULL)
    {
        fprintf(stderr, "remove_files_to_server: NULL Pointer Given: folder\n");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server;
    int server_fd = socket_client(&server);

    send_int(server_fd, 1);

    struct dirent *file = NULL;
    DIR *directory = open_dir(folder);

    int num_files;
    for (num_files = -2; (file = readdir(directory)) != NULL; ++num_files);

    directory = reopen_dir(directory, folder);

    send_int(server_fd, num_files);
    while ((file = readdir(directory)) != NULL)
    {
        if (cmp(file->d_name, ".") || cmp(file->d_name, ".."))
        {
            continue;
        }

        send_str(server_fd, file->d_name);
    }

    socket_close(server_fd);
    close_dir(directory);

    return;
}



int
main( const int    argc ,
      const char **argv )
{
    if (argc == 1)
    {
        folder_path = strndup(getenv("HOME"), HOME_LIMIT);
        int size = strlen(folder_path) + strlen(DEFAULT_FOLDER);
        folder_path = (char *) reallocate(folder_path, size, sizeof(char));
        strncat(folder_path, DEFAULT_FOLDER, size);
    }
    else if (argc == 2)
    {
        if (argv[ 1 ][ 0 ] == '/')
        {
            folder_path = strndup(argv[ 1 ], 256);
        }
        else
        {
            folder_path = strndup(getenv("HOME"), HOME_LIMIT);
            int size = strlen(folder_path) + strlen(argv[ 1 ]);
            folder_path = (char *) reallocate(folder_path, size, sizeof(char));
            strcat(folder_path, argv[ 1 ]);
        }
    }
    else
    {
        puts("usage: [path_to_default_folder]");
        return EXIT_FAILURE;
    }

    // puts("[[ Connecting to Server ]]");
    // struct sockaddr_in server;
    // int server_fd = socket_client(&server);
    
    puts("[[ Sending Files to Server ]]");
    send_files_to_server(folder_path);

    puts("[[ Initializing Readline ]]");
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
        free_mem(prompt);
        free_mem(input);
    }
    while (status);

    puts("\n[[ Removing Files from Server ]]");
    remove_files_from_server(folder_path);

    puts("[[ Freeing Memory ]]");
    free_mem(folder_path);

    puts("\n\n[[ Exited Normally ]]\n\n");
    return EXIT_SUCCESS;
}
