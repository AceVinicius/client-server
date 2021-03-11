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
#include <sys/types.h>
#include <sys/socket.h>

#include "../../lib/include/sockets.h"



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
send_character( int socket, char character )
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
send_string( int socket, char *string )
{
    size_t length = strlen(string);

    send_int(socket, length);

    int bytes_sent = send(socket, &string, sizeof(char) * length, 0);
    
    if (length != bytes_sent)
    {
        fprintf(stderr, "send_string: string not properly sent");
        // exit(EXIT_FAILURE);
    }
}



char *
recv_str( int socket )
{
    size_t length = recv_int(socket);

    char *string = allocate(length, sizeof(char));

    int bytes_recvd = recv(socket, string, sizeof(char) * length, 0);
    if (bytes_recvd == -1)
    {
        perror("recv_str");
        // exit(EXIT_FAILURE);
        return NULL;
    }
    else if (bytes_recvd != length)
    {
        perror("recv_str_size");
        // exit(EXIT_FAILURE);
        return NULL;
    }

    return string;
}
