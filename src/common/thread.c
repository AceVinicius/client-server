/**
 * @file sockets.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief Do operations with threads
 * @version 0.1
 * @date 2021-03-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void
create_thread( pthread_t *thread   ,
               void      *function ,
               void      *args     )
{
    int ret = pthread_create(thread, NULL, (void *) function, *args);
    if(ret)
    {
        perror("create_thread");
        exit( EXIT_FAILURE );
    }
}



void
wait_thread( pthread_t *thread )
{
    int ret = pthread_join(*thread, NULL);
    if(ret)
    {
        perror("wait_thread");
        exit( EXIT_FAILURE );
    }
}
