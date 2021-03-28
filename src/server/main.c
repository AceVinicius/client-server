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

#include "../../lib/include/sockets.h"



#define NUM_BYTES_TO_READ 500
#define PORT 2000



int
main( void )
{
    struct sockaddr_in *server;
    struct sockaddr_in *client;

    int server_fd = server_socket( server );

    if (listen(server_fd, 3) == -1)
    { 
        perror("Listen error");
        close_socket(&server_fd);
        exit(EXIT_FAILURE);
    }

    int client_fd = accept(server_fd, (struct sockaddr *) &client, (socklen_t *) sizeof(client));
    if (client_fd == -1)
    {
        perror("Accept error");
        close_socket(&server_fd);
        exit(EXIT_FAILURE);
    }

    while (client_fd)
    {
        int number = recv_int(client_fd);
        printf("received: %d\n", number);

        double pi = recv_double(client_fd);
        printf("received: %.13lf\n", pi);

        char letter = recv_char(client_fd);
        printf("received: %c\n", letter);

        char *string = recv_str(client_fd);
        printf("received: %s\n", string);

        close_socket(&client_fd);
        if ((client_fd = accept(server_fd, (struct sockaddr *) &client, (socklen_t*) sizeof(client))) < 0)
        {
            perror("accept");
            close_socket(&server_fd);
            exit(EXIT_FAILURE);
        }
    }

    close_socket(&server_fd);

    return EXIT_SUCCESS;
}
