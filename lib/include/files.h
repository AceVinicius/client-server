#ifndef CD_H
# define CD_H

# include <stdio.h>

# ifdef __cplusplus
extern c {
# endif



FILE *  open_file   ( const char *, const char * );
void    close_file  ( FILE * );



# ifdef __cplusplus
}
# endif


#endif // CD_H
