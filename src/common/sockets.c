/**
 * @file sockets.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief Do operations with sockets
 * @version 0.1
 * @date 2021-03-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../lib/include/server.h"
#include "../../lib/include/sockets.h"
#include "../../lib/include/allocation.h"



static void  wait_time   ( short );
static int   socket_new  ( struct sockaddr_in * );



/******************************************************************************
 ***                           INTERNAL FUNCTIONS                           ***
 ******************************************************************************/



static void
wait_time( short time )
{
    do
    {
        printf("Connection Failed: Retrying in %d seconds\r", time);
        fflush(stdout);
        sleep(1);
    }
    while (time--);

    return;
}



static int
socket_new( struct sockaddr_in *sock )
{
    const int new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (new_socket == -1)
    { 
        perror("socket");
        exit(EXIT_FAILURE);
    }

    sock->sin_family = AF_INET;
    sock->sin_port = htons(PORT);
    
    memset(sock->sin_zero, 0x0, 8);

    return new_socket;
}



/******************************************************************************
 ***                           EXTERNAL FUNCTIONS                           ***
 ******************************************************************************/



void
send_int( const int socket, const int number )
{
    if (send(socket, &number, sizeof(int), 0) == -1)
    {
        perror("send_int");
        exit(EXIT_FAILURE);
    }
}



int
recv_int( const int socket )
{
    int number;

    const int bytes_recvd = recv(socket, &number, sizeof(int), 0);
    if (bytes_recvd == -1)
    {
        perror("recv_int");
        exit(EXIT_FAILURE);
    }

    return number;
}



void
send_double( const int socket, const double number )
{
    if (send(socket, &number, sizeof(double), 0) == -1)
    {
        perror("send_double");
        exit(EXIT_FAILURE);
    }
}



double
recv_double( const int socket )
{
    double number;
    
    const int bytes_recvd = recv(socket, &number, sizeof(double), 0);
    if (bytes_recvd == -1)
    {
        perror("recv_double");
        exit(EXIT_FAILURE);
    }

    return number;
}



void
send_char( const int socket, const char character )
{
    if (send(socket, &character, sizeof(character), 0) == -1)
    {
        perror("send_character");
        exit(EXIT_FAILURE);
    }
}



char
recv_char( const int socket )
{
    char character;
    
    const int bytes_recvd = recv(socket, &character, sizeof(char), 0);
    if (bytes_recvd == -1)
    {
        perror("recv_char");
        exit(EXIT_FAILURE);
    }

    return character;
}



void
send_str( const int socket, const char *string )
{
    const long length = strlen(string);

    send_int(socket, length);

    const long bytes_sent = send(socket, string, sizeof(char) * length, 0);
    if (length != bytes_sent)
    {
        fprintf(stderr, "send_string: string not properly sent");
        exit(EXIT_FAILURE);
    }
}



char *
recv_str( const int socket )
{
    const long  length = recv_int(socket);
    char       *string = allocate(length, sizeof(char));

    const long bytes_recvd = recv(socket, string, sizeof(char) * length, 0);
    if (bytes_recvd == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    else if (bytes_recvd != length)
    {
        printf("%ld %ld\n", sizeof(char) * length, bytes_recvd);
        perror("recv_str_size_not_match");
        exit(EXIT_FAILURE);
    }

    return string;
}



int
socket_client( struct sockaddr_in *client )
{
    const int client_fd = socket_new(client);
    client->sin_addr.s_addr = inet_addr("127.0.0.1");

    for (short i = 0; i <= ATTEMPTS; ++i)
    {
        if (connect(client_fd, (struct sockaddr *) client, sizeof(*client)) != -1)
        {
            return client_fd;
        }

        wait_time(5);
    }

    printf("\t[ERROR] :: Unable to connect with server! %i attempts was made.\n\n", ATTEMPTS);
    exit(EXIT_SUCCESS);
}



int
socket_server( struct sockaddr_in *server )
{
    const int server_fd = socket_new(server);
    const int yes = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        socket_close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *) server, sizeof(*server)) == -1)
    { 
        perror("bind");
        socket_close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}



void
socket_listen( const int server_fd )
{
    if (listen(server_fd, SOCKET_MAX_QUEUE) == -1)
    {
        perror("listen");
        socket_close(server_fd);
        exit(EXIT_FAILURE);
    }

    return;
}



int
socket_accept( const int server_fd, struct sockaddr_in *client )
{
    int size = sizeof(struct sockaddr_in);
    int client_fd = accept(server_fd, (struct sockaddr *) client, (socklen_t *) &size);
    if (client_fd == -1)
    {
        perror("accept");
        socket_close(server_fd);
        exit(EXIT_FAILURE);
    }

    return client_fd;
}



void
socket_close( const int socket )
{
    if (close(socket) == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
