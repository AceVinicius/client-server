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
send_file( LIST *command )
{
    if (command->arguments == NULL)
    {
        fprintf(stderr, "send: missing file operand\n");
        puts("Try 'help send' for more information.");
        return 2;
    }

    struct sockaddr_in server;
    int server_fd = socket_client(&server);
    send_int(server_fd, 2);
    
    ARGUMENT *arg = command->arguments;

    while (arg != NULL)
    {
        char *tmp = allocate(CWD_LIMIT, sizeof(char));
        getcwd(tmp, CWD_LIMIT);

        // DIR *dp = NULL;
        // if ((dp = opendir(tmp)) == NULL)
        // {
        //     send_int(server_fd, 0);
        //     arg = arg->next;
        //     free_mem(tmp);
        //     continue;
        // }
        // closedir(dp);
        
        strcat(tmp, "/");
        strncat(tmp, arg->argument, 100);

        FILE *in = fopen(tmp, "r");
        if (in == NULL)
        {
            fprintf(stderr, "cp: cannot stat '%s': No such file or directory\n", arg->argument);
            send_int(server_fd, 0);
            arg = arg->next;
            free_mem(tmp);
            continue;
        }

        char *tmp2 = allocate(CWD_LIMIT, sizeof(char));
        strcpy(tmp2, "/home/acevinicius/Public/");
        strcat(tmp2, arg->argument);
        FILE *out = fopen(tmp2, "w");
        if (out == NULL)
        {
            fprintf(stderr, "bash: cannot create '%s' file\n", arg->argument);
            send_int(server_fd, 0);
            arg = arg->next;
            free_mem(tmp);
            continue;
        }
        
        char character;
        while (fscanf(in, "%c", &character) != EOF)
        {
            fprintf(out, "%c", character);
        }

        send_int(server_fd, 1);
        send_str(server_fd, arg->argument);

        fclose(in);
        fclose(out);
        free_mem(tmp);
        free_mem(tmp2);

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
    int server_fd = socket_client(&server);
    send_int(server_fd, 3);
    
    ARGUMENT *arg = command->arguments;

    while (arg != NULL)
    {
        char *tmp2 = allocate(CWD_LIMIT, sizeof(char));
        strcpy(tmp2, "/home/acevinicius/Public/");
        strcat(tmp2, arg->argument);
        if (remove(tmp2))
        {
            fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", arg->argument);
            send_int(server_fd, 0);
            arg = arg->next;
            continue;
        }

        send_int(server_fd, 1);
        send_str(server_fd, arg->argument);

        free_mem(tmp2);

        arg = arg->next;
    }

    send_int(server_fd, -1);
    socket_close(server_fd);

    return 0;
}



int
show( void )
{
    struct sockaddr_in server;
    int server_fd = socket_client(&server);
    send_int(server_fd, 4);

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
            error = send_file(command);
            break;

        case FETCH_CMD_ID:
            // error = fetch();
            break;

        case SHOW_CMD_ID:
            error = show();
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
