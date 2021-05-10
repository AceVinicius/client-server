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

#include "../../lib/include/thread.h"
#include "../../lib/include/server.h"
#include "../../lib/include/sockets.h"
#include "../../lib/include/hash-table.h"
#include "../../lib/include/allocation.h"



void
do_something( void *arg )
{
    const int socket_fd = (int) *arg;

    HASH_TABLE *table = create_hash_table();

    int num_files = recv_int(socket_fd);
    for (int i = 0; i < num_files; ++i)
    {
        char *file = recv_str(socket_fd);
        insert_hash_table(table, file, file);
        free_mem(file);
    }
    print_hash_table(table);

    num_files = recv_int(socket_fd);
    for (int i = 0; i < num_files; ++i)
    {
        char *file = recv_str(socket_fd);
        delete_hash_table(table, file);
        print_hash_table(table);
        free_mem(file);
    }

    destroy_hash_table(table);
}



int
main( void )
{
    struct sockaddr_in *server = (struct sockaddr_in *) allocate(0, sizeof(struct sockaddr_in));
    struct sockaddr_in *client = (struct sockaddr_in *) allocate(0, sizeof(struct sockaddr_in));

    const int server_fd = socket_server(server);

    socket_listen(server_fd);

    pthread_t threads[ MAX_THREADS ];

    while (1)
    {
        const int client_fd = socket_accept(server_fd, client);
        
        if (client_fd == 0) break;

        // pthread_create(&threads[ 0 ], NULL, do_something, client_fd);
        create_thread(&threads[ 0 ], do_something, &client_fd);

        socket_close(client_fd);
    }

    socket_close(server_fd);

    free_mem(server);
    free_mem(client);

    return EXIT_SUCCESS;
}
