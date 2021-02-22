#ifndef CLIENT_H
# define CLIENT_H



# define PROMPT_LIMIT 128
# define HOST_LIMIT    32



void  initialize_readline  ( void );
void  build_prompt         ( char * );
void  get_user_input       ( char * );
int   main                 ( /*const int, const char ***/ );



#endif // CLIENT_H
