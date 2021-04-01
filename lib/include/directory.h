#ifndef DIRECTORY_H
# define DIRECTORY_H



# include <dirent.h>



DIR *  open_dir   ( const char * );
void   close_dir  ( DIR * );



#endif // DIRECTORY_H
