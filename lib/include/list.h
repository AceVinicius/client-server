#ifndef LIST_H
# define LIST_H

# ifdef __cplusplus
extern c {
# endif



struct list
{
    char *command;              // 8 bytes
    char *path;                 // 8 bytes
    struct option *options;     // 8 bytes
    struct argument *arguments; // 8 bytes
    unsigned short port;        // 2 bytes
    int qtd_options;            // 4 bytes
    int qtd_arguments;          // 4 bytes
};

struct option
{
    struct option *next; // 8 bytes
    char *option;        // 8 bytes
};

struct argument
{
    struct argument *next; // 8 bytes
    char *argument;        // 8 bytes
};



typedef struct list LIST;
typedef struct option OPTION;
typedef struct argument ARGUMENT;



LIST *  initialize_list  ( void );
void    insert_option    ( LIST *, char * );
void    insert_argument  ( LIST *, char * );
void    free_command     ( LIST * );



# ifdef __cplusplus
}
# endif

#endif // LIST_H
