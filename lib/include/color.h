#ifndef COLOR_H
# define COLOR_H



# define BOLD        1
# define ITALIC      3
# define UNDERLINE   4
# define REGULAR     6
# define BACK        7
# define INVISIBLE   8
# define STRIKE      9

# define GREY       30
# define RED        31
# define GREEN      32
# define YELLOW     33
# define BLUE       34
# define PURPLE     35
# define CYAN       36
# define WHITE      37



void  start   ( void );
void  finish  ( void );
void  style   ( const int, const int );
void  reset   ( void );
void  place   ( const int, const int );



#endif // COLOR_H
