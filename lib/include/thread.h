#ifndef THREAD_H
# define THREAD_H



# define MAX_THREADS 25



# include <pthread.h>



void create_thread( pthread_t *, void *, void * );
void wait_thread( pthread_t * );



#endif // THREAD_H
