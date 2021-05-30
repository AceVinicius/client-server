#ifndef HISTORY_H
# define HISTORY_H

# include <stdio.h>

# define HISTORY_LIMIT 512

# ifdef __cplusplus
extern c {
# endif



FILE *  open_file             ( const char *, const char * );
void    close_file            ( FILE * );
void    add_input_to_history  ( const char * );
char *  get_history_path      ( void );
int     history               ( void );



# ifdef __cplusplus
}
# endif

#endif // HISTORY_H
