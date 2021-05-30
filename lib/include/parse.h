#ifndef PARSE_H
# define PARSE_H

# include "list.h"

# define TOKEN_LIMIT          16
# define WHITESPACE  " \t\r\n\v"

# ifdef __cplusplus
extern c {
# endif



void  parse  ( LIST *, char * );



# ifdef __cplusplus
}
# endif

#endif // PARSE_H
