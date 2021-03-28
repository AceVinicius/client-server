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
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../lib/include/sockets.h"
#include "../../lib/include/allocation.h"



#define PORT 2000 
#define ATTEMPTS 10





static void  wait_time  ( short time );



static void
wait_time( short time )
{
    while ((time--)+1)
    {
        printf("Connection Failed: Retrying in %d seconds\r", time);
        fflush(stdout);
        sleep(1);
    }
}



void
send_int( int socket, int number )
{
    if (send(socket, &number, sizeof(int), 0) == -1)
    {
        perror("send_int");
        exit(EXIT_FAILURE);
    }
}



int
recv_int( int socket )
{
    int number;

    int bytes_recvd = recv(socket, &number, sizeof(int), 0);
    if (bytes_recvd == -1)
    {
        perror("recv_int");
        // exit(EXIT_FAILURE);
    }

    return number;
}



void
send_double( int socket, double number )
{
    if (send(socket, &number, sizeof(double), 0) == -1)
    {
        perror("send_double");
        exit(EXIT_FAILURE);
    }
}



double
recv_double( int socket )
{
    double number;
    
    int bytes_recvd = recv(socket, &number, sizeof(double), 0);
    if (bytes_recvd == -1)
    {
        perror("recv_double");
        // exit(EXIT_FAILURE);
    }

    return number;
}



void
send_char( int socket, char character )
{
    if (send(socket, &character, sizeof(character), 0) == -1)
    {
        perror("send_character");
        exit(EXIT_FAILURE);
    }
}



char
recv_char( int socket )
{
    char character;
    
    int bytes_recvd = recv(socket, &character, sizeof(char), 0);
    if (bytes_recvd == -1)
    {
        perror("recv_char");
        // exit(EXIT_FAILURE);
    }

    return character;
}



void
send_str( int socket, char *string )
{
    long length = strlen(string);

    send_int(socket, length);

    long bytes_sent = send(socket, string, sizeof(char) * length, 0);
    if (length != bytes_sent)
    {
        fprintf(stderr, "send_string: string not properly sent");
        // exit(EXIT_FAILURE);
    }
}



char *
recv_str( int socket )
{
    long length = recv_int(socket);

    char *string = allocate(length, sizeof(char));

    long bytes_recvd = recv(socket, string, sizeof(char) * length, 0);
    if (bytes_recvd == -1)
    {
        perror("recv_str");
        // exit(EXIT_FAILURE);
        return NULL;
    }
    else if (bytes_recvd != length)
    {
        perror("recv_str_size_not_match");
        // exit(EXIT_FAILURE);
        return NULL;
    }

    return string;
}



int
client_socket( struct sockaddr_in *serv_addr )
{
    int new_socket = 0;

    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr->sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    for (short i = 0; connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0; ++i)
    {
        wait_time(5);

        if (i >= ATTEMPTS)
        {
            printf("Connection Failed: %d attempts was made before quitting\n", ATTEMPTS);
            exit(EXIT_SUCCESS);
        }
    }

    return new_socket;
}



int
server_socket( struct sockaddr_in *server )
{
    // Creating socket file descriptor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    { 
        perror("Can't create socket");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Server socket created with fd: %d\n", server_fd);


    // Handle the error of the port already in use
    int yes = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    { 
        perror("socket options error");
        close_socket( &server_fd );
        exit(EXIT_FAILURE);
    }
    // server->sin_family = AF_INET;
    // server->sin_port = htons( PORT );
    // memset(server->sin_zero, 0x0, 8);
    // server->sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to a port
    if (bind(server_fd, (struct sockaddr *) &server, sizeof(server)) == -1)
    { 
        perror("socket bind error");
        close_socket( &server_fd );
        exit(EXIT_FAILURE);
    }

    return server_fd;
}



void
close_socket( int *socket )
{
    close(*socket);
    *socket = -1;
}
