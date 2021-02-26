#ifndef CLIENT_H
# define CLIENT_H



# define PROMPT_LIMIT 128
# define HOST_LIMIT    32

# define STRUCT
# define DEBUG



void  initialize_readline  ( void );
void  build_prompt         ( char * );
void  get_user_input       ( const char * );
int   main                 ( void );



#endif // CLIENT_H
