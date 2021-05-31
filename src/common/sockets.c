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
#include "../../lib/include/files.h"


static void  wait_time   ( short );
static int   socket_new  ( struct sockaddr_in *, const int );



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
socket_new( struct sockaddr_in *sock, const int port )
{
    const int new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (new_socket == -1)
    { 
        perror("socket");
        exit(EXIT_FAILURE);
    }

    sock->sin_family = AF_INET;
    sock->sin_port = htons(port);
    
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



void
send_file( const int   socket ,
           const char *file   )
{
    char data[BUFFER_SIZE] = { 0 };

    FILE *fp = open_file(file, "r");
    while(fgets(data, BUFFER_SIZE, fp) != NULL)
    {
        if (send(socket, data, sizeof(data), 0) == -1)
        {
            perror("Error in sending file.");
            exit(EXIT_FAILURE);
        }

        explicit_bzero(data, BUFFER_SIZE);
    }

    close_file(fp);
}



void
recv_file( const int   socket ,
           const char *file   )
{
    int n;
    char buffer[BUFFER_SIZE];

    char *full_path = strndup(getenv("HOME"), 256);
    strcat(full_path, "/Documents");
    strcat(full_path, file);

    FILE *fp = open_file(full_path, "w");

    while (1)
    {
        n = recv(socket, buffer, BUFFER_SIZE, 0);

        if (n <= 0)
        {
            break;
            return;
        }

        fprintf(fp, "%s", buffer);
        explicit_bzero(buffer, BUFFER_SIZE);
    }
}



int
socket_client( struct sockaddr_in *client, const char *ip, const int port )
{
    const int client_fd = socket_new(client, port);
    client->sin_addr.s_addr = inet_addr(ip);

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
socket_server( struct sockaddr_in *server, const int port )
{
    const int server_fd = socket_new(server, port);
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
