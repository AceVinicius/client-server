#ifndef SOCKETS_H
# define SOCKETS_H



void    send_int        ( int, int );
void    send_double     ( int, double );
void    send_character  ( int, char );
void    send_string     ( int, char * );
int     recv_int        ( int );
double  recv_double     ( int );
char    recv_char       ( int );
char *  recv_str        ( int );



#endif // SOCKETS_H