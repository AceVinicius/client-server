#ifndef CLIENT_H
# define CLIENT_H



# define USER_LIMIT    32
# define HOST_LIMIT    64
# define CWD_LIMIT     64
# define PROMPT_LIMIT 168



void  initialize_readline  ( void );
void  build_prompt         ( char * );
void  get_user_input       ( const char * );
int   main                 ( void );



#endif // CLIENT_H
