#ifndef QUEUE_H
# define QUEUE_H

# include <stdio.h>
# include <stdbool.h>

# define NON_THREAD_SAFE  0
# define THREAD_SAFE      1
# define COND_SAFE        2

# ifdef __cplusplus
extern c {
# endif



typedef struct queue_t QUEUE;



QUEUE *  queue_create    ( const size_t, short );
bool     queue_is_empty  ( QUEUE * );
bool     enqueue         ( QUEUE *, const void * );
void*    dequeue         ( QUEUE * );
bool     queue_destroy   ( QUEUE *queue );



# ifdef __cplusplus
}
# endif

#endif //   QUEUE_H
