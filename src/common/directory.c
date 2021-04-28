/**
 * @file directory.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../lib/include/directory.h"



DIR *
open_dir( const char *folder )
{
    DIR *directory = opendir((const char *) folder);
    if (directory == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    return directory;
}



void
close_dir( DIR *directory )
{
    if (directory == NULL)
    {

    }
    
    if (closedir(directory) == -1)
    {
        perror("closedir");
        exit(EXIT_FAILURE);
    }

    directory = NULL;
}
