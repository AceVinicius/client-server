#ifndef DIRECTORY_H
# define DIRECTORY_H

# include <dirent.h>

# ifdef __cplusplus
extern c {
# endif



DIR *  open_dir    ( const char * );
DIR *  reopen_dir  ( DIR *, const char * );
void   close_dir   ( DIR * );



# ifdef __cplusplus
}
# endif

#endif // DIRECTORY_H
