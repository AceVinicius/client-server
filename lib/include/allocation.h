#ifndef ALLOCATE_H
# define ALLOCATE_H

# ifdef __cplusplus
extern c {
# endif



void *  allocate    ( const size_t, const size_t );
void *  reallocate  ( void *, const size_t, const size_t );
char *  strndup     ( const char *, const size_t );
void    free_mem    ( void * );



# ifdef __cplusplus
}
# endif

#endif // ALLOCATE_H
