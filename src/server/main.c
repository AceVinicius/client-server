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
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../lib/include/queue.h"
#include "../../lib/include/thread.h"
#include "../../lib/include/server.h"
#include "../../lib/include/sockets.h"
#include "../../lib/include/hash-table.h"
#include "../../lib/include/allocation.h"



QUEUE *clients;
HASH_TABLE *filesystem;
struct sockaddr_in server;
pthread_t threads[ THREAD_POOL_SIZE ];
pthread_t enqueue_connections;

volatile bool running = true;



void
handle_client( DATA *client )
{
    switch (client->cmd)
    {
        case 0:
        {
            int num_files = recv_int(client->fd);

            for (int i = 0; i < num_files; ++i)
            {
                client->file = recv_str(client->fd);
                hash_table_insert(filesystem, client->file, client);
                free_mem(client->file);
            }

            hash_table_print(filesystem);

            break;
        }

        case 1:
        {
            int num_files = recv_int(client->fd);

            for (int i = 0; i < num_files; ++i)
            {
                client->file = recv_str(client->fd);
                hash_table_delete(filesystem, client->file);
                free_mem(client->file);
            }

            hash_table_print(filesystem);

            break;
        }

        case 2:
        {
            int is_valid;

            do
            {    
                is_valid = recv_int(client->fd);

                if (is_valid == 1)
                {
                    client->file = recv_str(client->fd);
                    hash_table_insert(filesystem, client->file, client);
                    free_mem(client->file);
                }
            }
            while (is_valid != -1);

            hash_table_print(filesystem);
            
            break;
        }

        case 3:
        {
            int is_valid;

            do
            {    
                is_valid = recv_int(client->fd);

                if (is_valid == 1)
                {
                    client->file = recv_str(client->fd);
                    hash_table_delete(filesystem, client->file);
                    free_mem(client->file);
                }
            }
            while (is_valid != -1);

            hash_table_print(filesystem);
            
            break;
        }

        case 4:
        {
            hash_table_list(filesystem, client->fd);

            break;
        }

        case 5:
        {
            int is_valid;

            do
            {    
                is_valid = recv_int(client->fd);

                if (is_valid == 1)
                {
                    client->file = recv_str(client->fd);
                    
                    DATA *data = hash_table_peek(filesystem, client->file);
                    if (data == NULL)
                    {
                        send_int(client->fd, 1);
                    }
                    else
                    {
                        send_int(client->fd, 0);
                        send_str(client->fd, data->ip);
                        send_int(client->fd, data->port);
                    }

                    free_mem(client->file);
                }
            }
            while (is_valid != -1);


            break;
        }

        default:
        {
            break;
        }
    }

    printf("[[ Closing connection with %s in Socket %i ]]\n", client->ip, client->fd);
    socket_close(client->fd);
}



void *
handle_client_queue( void *nothing )
{
    DATA *client;

    while (running)
    {
        client = (DATA *) dequeue(clients);

        if (client != NULL)
        {
            handle_client(client);
            free_mem(client);
        }
    }

    free_mem(client);

    return NULL;
}


 
void *
handle_connections( void *nothing )
{
    puts("[[ Binding Server to a Socket ]]");
    const int server_fd = socket_server(&server, 8080);

    while (running)
    {
        socket_listen(server_fd);
        
        puts("[[ connecting to Client ]]");

        struct sockaddr_in client_t;
        DATA *client = (DATA *) allocate(0, sizeof(DATA));

        client->fd   = socket_accept(server_fd, &client_t);
        client->ip   = inet_ntoa(client_t.sin_addr);
        client->cmd  = recv_int(client->fd);
        client->port = recv_int(client->fd);

        if (enqueue(clients, client))
        {
            printf("[[ Enqueued Connection with %s in Socket %i ]]\n", client->ip, client->fd);
        }
        else
        {
            printf("[[ Failed to connect with %s in Socket %i ]]\n", client->ip, client->fd);
        }
    }

    return NULL;
}



int
main( void )
{
    puts("[[ Loading Filesystem ]]");
    filesystem = hash_table_create(8, sizeof(DATA));

    puts("[[ Loading Client Queue ]]");
    clients = queue_create(sizeof(DATA), COND_SAFE);

    puts("[[ Loading Threads ]]");
    for (size_t i = 0; i < THREAD_POOL_SIZE; ++i)
    {
        thread_create(&threads[ i ], handle_client_queue, NULL);
    }
    thread_create(&enqueue_connections, handle_connections, NULL);

    while (running)
    {
        printf("\n>>> ");
        
        int go;
        scanf(" %d", &go);

        if (!go) running = false;
    }

    puts("\n[[ Closing Server ]]");

    puts("[[ Cleaning Filesystem ]]");
    hash_table_destroy(filesystem);

    puts("[[ Waiting for Threads to Join ]]");
    for (size_t i = 0; i < THREAD_POOL_SIZE; ++i)
    {
        pthread_kill(&threads[ i ]);
    }

    puts("[[ Cleaning Client Queue ]]");
    queue_destroy(clients);

    puts("\n\n[[ Exited Normally ]]\n\n");
    return EXIT_SUCCESS;
}
