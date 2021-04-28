#ifndef ALLOCATE_H
# define ALLOCATE_H



void *  allocate     ( const size_t, const size_t );
void *  reallocate   ( void *, const size_t, const size_t );
char *  duplicate    ( const char * );
char *  duplicate_n  ( const char *, const size_t );
void    free_mem     ( void * );



#endif // ALLOCATE_H
