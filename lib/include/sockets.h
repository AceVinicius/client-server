#ifndef SOCKETS_H
# define SOCKETS_H



void    send_int       ( int, int );
void    send_double    ( int, double );
void    send_char      ( int, char );
void    send_str       ( int, char * );
int     recv_int       ( int );
double  recv_double    ( int );
char    recv_char      ( int );
char *  recv_str       ( int );
int     client_socket  ( struct sockaddr_in * );
int     server_socket  ( struct sockaddr_in * );
void    close_socket   ( int * );

#endif // SOCKETS_H
