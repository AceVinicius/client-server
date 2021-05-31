#ifndef SOCKETS_H
# define SOCKETS_H

# include <arpa/inet.h>

# define ATTEMPTS             15
# define BUFFER_SIZE        1024
# define SERVER_IP   "127.0.0.1"
# define SERVER_PORT        8080
# define CLIENT_PORT        8079

# ifdef __cplusplus
extern c {
# endif



void    send_int       ( const int, const int );
void    send_double    ( const int, const double );
void    send_char      ( const int, const char );
void    send_str       ( const int, const char * );
void    send_file      ( const int, const char * );     
int     recv_int       ( const int );
double  recv_double    ( const int );
char    recv_char      ( const int );
char *  recv_str       ( const int );
void    recv_file      ( const int, const char * );
int     socket_client  ( struct sockaddr_in *, const char *, const int );
int     socket_server  ( struct sockaddr_in *, const int );
void    socket_listen  ( const int );
int     socket_accept  ( const int, struct sockaddr_in * );
void    socket_close   ( const int );



# ifdef __cplusplus
}
# endif

#endif // SOCKETS_H
