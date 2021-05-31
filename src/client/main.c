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
#include <pthread.h>
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
#include "../../lib/include/thread.h"



/**
 * @brief User input
 */
static char *input = (char *) NULL;
static char *g_folder_path = (char *) NULL;
static unsigned short client_port;
static struct sockaddr_in server;
static pthread_t file_thread;


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
    int server_fd = socket_client(&server, SERVER_IP, SERVER_PORT);

    send_int(server_fd, 0);
    send_int(server_fd, client_port);

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
    int server_fd = socket_client(&server, SERVER_IP, SERVER_PORT);

    send_int(server_fd, 1);
    send_int(server_fd, client_port);

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



void *
send_requested_file( void *nothing )
{
    puts("[[ Binding Server to a Socket ]]");
    const int server_fd = socket_server(&server, CLIENT_PORT);
    client_port = ntohs(server.sin_port);
    
    while (1)
    {
        socket_listen(server_fd);
        
        struct sockaddr_in client_t;
        const int client_fd = socket_accept(server_fd, &client_t);
        
        char *full_path = (char *) allocate(CWD_LIMIT, sizeof(char));
        strcat(full_path, g_folder_path);
        strcat(full_path, "/");
        strcat(full_path, recv_str(client_fd));

        send_file(client_fd, full_path);

        socket_close(client_fd);
    }

    return NULL;
}



int
main( const int    argc ,
      const char **argv )
{
    thread_create(&file_thread, send_requested_file, NULL);

    if (argc == 1)
    {
        g_folder_path = strndup(getenv("HOME"), HOME_LIMIT);
        int size = strlen(g_folder_path) + strlen(DEFAULT_FOLDER);
        g_folder_path = (char *) reallocate(g_folder_path, size, sizeof(char));
        strncat(g_folder_path, DEFAULT_FOLDER, size);
    }
    else if (argc == 2)
    {
        if (argv[ 1 ][ 0 ] == '/')
        {
            g_folder_path = strndup(argv[ 1 ], 256);
        }
        else
        {
            g_folder_path = strndup(getenv("HOME"), HOME_LIMIT);
            int size = strlen(g_folder_path) + strlen(argv[ 1 ]);
            g_folder_path = (char *) reallocate(g_folder_path, size, sizeof(char));
            strcat(g_folder_path, argv[ 1 ]);
        }
    }
    else
    {
        puts("usage: [path_to_default_folder]");
        return EXIT_FAILURE;
    }
    
    puts("[[ Initializing Readline ]]");
    initialize_readline();

    sleep(1);

    puts("[[ Sending Files to Server ]]");
    send_files_to_server(g_folder_path);

    bool status = true;

    do
    {
        char *prompt = (char *) allocate(PROMPT_LIMIT, sizeof(char));

        build_prompt(prompt);
        get_user_input(prompt);

        LIST *command = initialize_list();
        parse(command, input);
        command->path = strndup(g_folder_path, CWD_LIMIT);
        command->port = client_port;
        status = execute(command);

        free_command(command);
        free_mem(prompt);
        free_mem(input);
    }
    while (status);

    puts("\n[[ Removing Files from Server ]]");
    remove_files_from_server(g_folder_path);

    puts("[[ Freeing Memory ]]");
    free_mem(g_folder_path);

    puts("\n\n[[ Exited Normally ]]\n\n");
    return EXIT_SUCCESS;
}
