/**
 * @file execute.c
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
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#include "../../lib/include/history.h"
#include "../../lib/include/list.h"
#include "../../lib/include/cd.h"
#include "../../lib/include/ls.h"
#include "../../lib/include/execute.h"
#include "../../lib/include/general.h"
#include "../../lib/include/client.h"
#include "../../lib/include/allocation.h"
#include "../../lib/include/sockets.h"



static int  read_command  ( const char * );



/******************************************************************************
 ***                           INTERNAL FUNCTIONS                           ***
 ******************************************************************************/



/**
 * @brief Receive a word and returns an id
 *
 * @param command Word to be identified
 * @return int ID of the command
 */
static int
read_command( const char *command )
{
    if (command == EMPTY_CMD)
    {
        return EMPTY_CMD_ID;
    }
    else if (cmp(command, EXIT_CMD))
    {
        return EXIT_CMD_ID;
    }
    else if (cmp(command, HISTORY_CMD))
    {
        return HISTORY_CMD_ID;
    }
    else if (cmp(command, STAT_CMD))
    {
        return STAT_CMD_ID;
    }
    else if (cmp(command, REMOVE_CMD))
    {
        return REMOVE_CMD_ID;
    }
    else if (cmp(command, SEND_CMD))
    {
        return SEND_CMD_ID;
    }
    else if (cmp(command, FETCH_CMD))
    {
        return FETCH_CMD_ID;
    }
    else if (cmp(command, LS_CMD))
    {
        return LS_CMD_ID;
    }
    else if (cmp(command, CD_CMD))
    {
        return CD_CMD_ID;
    }
    else if (cmp(command, SHOW_CMD))
    {
        return SHOW_CMD_ID;
    }

    return UNKNOWN_CMD_ID;
}



int
send_file_s( LIST *command )
{
    if (command->arguments == NULL)
    {
        fprintf(stderr, "send: missing file operand\n");
        puts("Try 'help send' for more information.");
        return 2;
    }

    struct sockaddr_in server;
    int server_fd = socket_client(&server, SERVER_IP, SERVER_PORT);
    send_int(server_fd, 2);
    send_int(server_fd, command->port);
    
    ARGUMENT *arg = command->arguments;

    while (arg != NULL)
    {
        char *file = (char *) allocate(CWD_LIMIT, sizeof(char));

        strncpy(file, arg->argument, CWD_LIMIT);

        FILE *in = fopen(file, "r");
        if (in == NULL)
        {
            fprintf(stderr, "cp: cannot stat '%s': No such file or directory\n", arg->argument);
            send_int(server_fd, 0);
            free_mem(file);
            arg = arg->next;
            continue;
        }

        char *full_path = (char *) allocate(CWD_LIMIT, sizeof(char));
        char *file_name = (char *) allocate(CWD_LIMIT, sizeof(char));
        
        strncpy(full_path, command->path, CWD_LIMIT);
        file_name = strrchr(arg->argument, '/');

        if (file_name == NULL)
        {
            strncat(full_path, "/", CWD_LIMIT - strnlen(full_path, CWD_LIMIT));
            strncat(full_path, arg->argument, CWD_LIMIT - strnlen(arg->argument, CWD_LIMIT));
        }
        else
        {
            strncat(full_path, file_name, CWD_LIMIT - strnlen(file_name, CWD_LIMIT));
        }

        FILE *out = fopen(full_path, "w");
        if (out == NULL)
        {
            fprintf(stderr, "bash: cannot create '%s' file\n", arg->argument);
            send_int(server_fd, 0);
            free_mem(full_path);
            fclose(in);
            arg = arg->next;
            continue;
        }
        
        char character;
        while (fscanf(in, "%c", &character) != EOF)
        {
            fprintf(out, "%c", character);
        }

        send_int(server_fd, 1);
        send_str(server_fd, strrchr(arg->argument, '/') + 1);

        fclose(in);
        fclose(out);
        free_mem(full_path);

        arg = arg->next;
    }

    send_int(server_fd, -1);
    socket_close(server_fd);

    return 0;
}


int
remove_file( LIST *command )
{
    if (command->arguments == NULL)
    {
        fprintf(stderr, "rm: missing operand\n");
        puts("Try 'help rm' for more information.");
        return 2;
    }

    struct sockaddr_in server;
    int server_fd = socket_client(&server, SERVER_IP, SERVER_PORT);
    send_int(server_fd, 3);
    send_int(server_fd, command->port);
    
    ARGUMENT *arg = command->arguments;

    while (arg != NULL)
    {
        char *full_path = (char *) allocate(CWD_LIMIT, sizeof(char));
        char *file_name = (char *) allocate(CWD_LIMIT, sizeof(char));
        
        strncpy(full_path, command->path, CWD_LIMIT);
        file_name = strrchr(arg->argument, '/');

        if (file_name == NULL)
        {
            strncat(full_path, "/", CWD_LIMIT - strnlen(full_path, CWD_LIMIT));
            strncat(full_path, arg->argument, CWD_LIMIT - strnlen(arg->argument, CWD_LIMIT));
        }
        else
        {
            strncat(full_path, file_name, CWD_LIMIT - strnlen(file_name, CWD_LIMIT));
        }

        if (remove(full_path))
        {
            fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", arg->argument);
            send_int(server_fd, 0);
            free_mem(full_path);
            arg = arg->next;
            continue;
        }

        send_int(server_fd, 1);
        send_str(server_fd, arg->argument);

        free_mem(full_path);

        arg = arg->next;
    }

    send_int(server_fd, -1);
    socket_close(server_fd);

    return 0;
}



int
show( LIST *command )
{
    struct sockaddr_in server;
    int server_fd = socket_client(&server, SERVER_IP, SERVER_PORT);
    send_int(server_fd, 4);
    send_int(server_fd, command->port);

    int receiving;

    do
    {
        receiving = recv_int(server_fd);

        if (receiving == 1)
        {
            char *tmp = recv_str(server_fd);
            puts(tmp);
            free_mem(tmp);
        }
    }
    while (receiving != -1);

    socket_close(server_fd);

    return 0;
}



int
fetch( LIST *command )
{
    if (command->arguments == NULL)
    {
        fprintf(stderr, "fetch: missing operand\n");
        puts("Try 'help fetch' for more information.");
        return 2;
    }

    ARGUMENT *arg = command->arguments;
    struct sockaddr_in server;
    int server_fd = socket_client(&server, SERVER_IP, SERVER_PORT);
    send_int(server_fd, 5);
    send_int(server_fd, command->port);

    while (arg != NULL)
    {
        send_int(server_fd, 1);
        send_str(server_fd, arg->argument);
        
        if (recv_int(server_fd))
        {
            arg = arg->next;
            continue;
        }

        char *client_ip = recv_str(server_fd);
        int client_port = recv_int(server_fd);

        printf("ip: %s - port: %hu\n", client_ip, client_port);

        struct sockaddr_in client;
        int client_fd = socket_client(&client, client_ip, client_port);

        send_str(client_fd, arg->argument);
        recv_file(client_fd, arg->argument);

        socket_close(client_fd);
        free_mem(client_ip);

        arg = arg->next;
    }

    send_int(server_fd, -1);
    socket_close(server_fd);

    return 0;
}



/******************************************************************************
 ***                           EXTERNAL FUNCTIONS                           ***
 ******************************************************************************/



/**
 * @brief Call the proper function based in the user input
 *
 * @param command List of operation and parameters
 * @return int Returns true if the program will run again or 0 if user wants
 *             to terminate
 */
int
execute( LIST *command )
{
    const char *cmd = command->command;
    const int cmd_id = read_command(cmd);

    int error = 0;

    switch (cmd_id)
    {
        case EMPTY_CMD_ID:
            break;

        case EXIT_CMD_ID:
            return EXIT_CLIENT;
            break;

        case HISTORY_CMD_ID:
            error = history();
            break;

        case STAT_CMD_ID:
            break;

        case REMOVE_CMD_ID:
            error = remove_file(command);
            break;

        case SEND_CMD_ID:
            error = send_file_s(command);
            break;

        case FETCH_CMD_ID:
            error = fetch(command);
            break;

        case SHOW_CMD_ID:
            error = show(command);
            break;

        case LS_CMD_ID:
            error = ls(command);
            break;

        case CD_CMD_ID:
            error = cd(command);
            break;

        default:
            fprintf(stderr, "client: command not found: %s\n", cmd);
            break;
    }

    if (error)
    {
        // TODO: Do something
    }

    return CONTINUE_CLIENT;
}
