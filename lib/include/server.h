#ifndef SERVER_H
# define SERVER_H

# define NUM_BYTES_TO_READ  500
# define PORT              8080
# define SOCKET_MAX_QUEUE     1
# define THREAD_POOL_SIZE    24

# ifdef __cplusplus
extern c {
# endif



struct data_t
{
    int fd;
    int cmd;
    unsigned short port;
    char *ip;
    char *file;
};



typedef struct data_t DATA;



# ifdef __cplusplus
}
# endif

#endif // SERVER_H
