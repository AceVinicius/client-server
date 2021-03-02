/**
 * @file color.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief 
 * @version 0.1
 * @date 2021-03-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>

#include "../../lib/include/color.h"



void
style( const int type  ,
       const int color )
{
    printf("\033[%d;%dm", type, color);
}



void
reset( void )
{
    printf("\033[0;0m");
}



void
place( const int x ,
       const int y )
{
    printf("\033[%d;%dH", y+1, x+1);
}
