/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../lib/include/server.h"
#include "../../lib/include/sockets.h"



int
main( void )
{
    struct sockaddr_in server;
    struct sockaddr_in client;

    const int server_fd = socket_server(&server);

    socket_listen(server_fd);

    while (1)
    {
        const int client_fd = socket_accept(server_fd, &client);
        
        if (client_fd == 0) break;

        // do some stuff

        socket_close(client_fd);
    }

    socket_close(server_fd);

    return EXIT_SUCCESS;
}