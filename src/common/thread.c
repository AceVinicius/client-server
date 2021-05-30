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

#include "../../lib/include/thread.h"



void
thread_create( pthread_t *thread             ,
               void      *(*function)(void *) ,
               void      *args               )
{
    int ret = pthread_create(thread, NULL, function, args);
    if(ret)
    {
        perror("thread_create");
        exit( EXIT_FAILURE );
    }
}



void
thread_wait( pthread_t *thread )
{
    int ret = pthread_join(*thread, NULL);
    if(ret)
    {
        perror("thread_wait");
        exit( EXIT_FAILURE );
    }
}



void
mutex_create( pthread_mutex_t *mutex )
{
    int ret = pthread_mutex_init(mutex, NULL);
    if (ret != 0)
    {
        perror("mutex_create");
        exit(EXIT_FAILURE);
    }
}



void
mutex_lock( pthread_mutex_t *mutex )
{
    int ret = pthread_mutex_lock(mutex);
    if (ret != 0)
    {
        perror("mutex_lock");
        exit(EXIT_FAILURE);
    }
}



void
mutex_unlock( pthread_mutex_t *mutex )
{
    int ret = pthread_mutex_unlock(mutex);
    if (ret != 0)
    {
        perror("mutex_unlock");
        exit(EXIT_FAILURE);
    }
}



void
mutex_destroy( pthread_mutex_t *mutex )
{
    int ret = pthread_mutex_destroy(mutex);
    if (ret != 0)
    {
        perror("mutex_destroy");
        // exit(EXIT_FAILURE);
    }
}



void
cond_create( pthread_cond_t *cond )
{
    int ret = pthread_cond_init(cond, NULL);
    if (ret != 0)
    {
        perror("cond_create");
        exit(EXIT_FAILURE);
    }
}



void
cond_wait( pthread_cond_t *cond, pthread_mutex_t *mutex )
{
    int ret = pthread_cond_wait(cond, mutex);
    if (ret != 0)
    {
        perror("cond_wait");
        exit(EXIT_FAILURE);
    }
}



void
cond_signal( pthread_cond_t *cond )
{
    int ret = pthread_cond_signal(cond);
    if (ret != 0)
    {
        perror("cond_signal");
        exit(EXIT_FAILURE);
    }
}



void
cond_destroy( pthread_cond_t *cond )
{
    int ret = pthread_cond_destroy(cond);
    if (ret != 0)
    {
        perror("cond_destroy");
        exit(EXIT_FAILURE);
    }
}
