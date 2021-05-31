#ifndef CLIENT_H
# define CLIENT_H

# define USER_LIMIT            32
# define HOST_LIMIT            64
# define PROMPT_LIMIT         168
# define HOME_LIMIT            64

# define DEFAULT_FOLDER "/Public"

# ifdef __cplusplus
extern c {
# endif



void  initialize_readline  ( void );
void  build_prompt         ( char * );
void  get_user_input       ( const char * );



# ifdef __cplusplus
}
# endif

#endif // CLIENT_H
