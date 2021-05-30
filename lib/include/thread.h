#ifndef THREAD_H
# define THREAD_H

# include <pthread.h>

# ifdef __cplusplus
extern c {
# endif



extern void  thread_create  ( pthread_t *, void *(*)(void *), void * );
extern void  thread_wait    ( pthread_t * );

extern void  mutex_create   ( pthread_mutex_t * );
extern void  mutex_lock     ( pthread_mutex_t * );
extern void  mutex_unlock   ( pthread_mutex_t * );
extern void  mutex_destroy  ( pthread_mutex_t * );

extern void  cond_create   ( pthread_cond_t * );
extern void  cond_wait     ( pthread_cond_t *, pthread_mutex_t * );
extern void  cond_signal   ( pthread_cond_t * );
extern void  cond_destroy  ( pthread_cond_t * );



# ifdef __cplusplus
}
# endif

#endif // THREAD_H
