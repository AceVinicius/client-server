#ifndef HISTORY_H
# define HISTORY_H



# define HISTORY_LIMIT 512



FILE *  open_file             ( const char *, const char * );
void    close_file            ( FILE * );
void    add_input_to_history  ( char * );
char *  get_history_path      ( void );
int     history               ( void );



#endif // HISTORY_H
