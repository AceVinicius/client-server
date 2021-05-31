#ifndef GENERAL_H
# define GENERAL_H

# define CWD_LIMIT 256

# ifdef __cplusplus
extern c {
# endif



char *  gethome    ( void );
int     cmp        ( const char *, const char * );
size_t  strnlen    ( const char *, size_t );



# ifdef __cplusplus
}
# endif

#endif // GENERAL_H
