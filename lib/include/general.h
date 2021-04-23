#ifndef GENERAL_H
# define GENERAL_H



char *  gethome   ( void );
char *  strdup    ( const char * );
int     cmp       ( const char *, const char * );

extern char *  strdup  ( const char * );
extern char *  strndup  ( const char *, const size_t );



#endif // GENERAL_H
