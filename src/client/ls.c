/**
 * @file ls.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief 
 * @version 0.1
 * @date 2021-03-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../lib/include/allocation.h"
#include "../../lib/include/list.h"
#include "../../lib/include/execute.h"
#include "../../lib/include/color.h"
#include "../../lib/include/ls.h"





int
ls( LIST *command )
{
    unsigned int num_files = 0;
    unsigned int max_size  = 0;
    unsigned int count     = 0;

    bool all_content = false;
    // bool file_print  = false;
    struct dirent *dptr = NULL;
    long *ptr = NULL;
    char curr_dir[ PWD_LIMIT ];
    DIR *dp = NULL;

    OPTION *aux = command->options;
    for (size_t i = 0; aux != NULL; ++i)
    {
        if (!strcmp(aux->option, "-a"))
        {
            all_content = true;
        }
        aux = aux->next;
    }

    if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
    {
        fprintf(stderr, "bash: couldn't get current directory\n");
        free_mem(ptr);
        exit(EXIT_FAILURE);
    }

    dp = opendir((const char*) curr_dir);

    if (dp == NULL)
    {
        fprintf(stderr, "bash: Could not open the working directory\n");
        free_mem(ptr);
        exit(EXIT_FAILURE);
    }

    while ((dptr = readdir(dp)) != NULL)
    {
        if (all_content)
        {
            num_files++;
        }
        else
        {
            if ((dptr -> d_name[ 0 ]) != '.')
            {
                num_files++;
            }
        }
    }

    closedir(dp);
    dptr = NULL;
    dp   = NULL;

    if (!num_files)
    {
        return 2;
    }
    else
    {
        ptr = (long *) malloc(sizeof(void *) * num_files);

        if (!ptr)
        {
            fprintf(stderr, "bash: 'ptr' allocation error\n");
            free_mem(ptr);
            exit(EXIT_FAILURE);
        }
        else 
        {
            memset(ptr, 0, sizeof(void *) * num_files);
        }
    }

    dp = opendir((const char *) curr_dir);

    if (dp == NULL)
    {
        fprintf(stderr, "bash: Could not open the working directory\n");
        free_mem( ptr );
        exit(EXIT_FAILURE);
    }

    unsigned int j = 0;
    for (count = 0; (dptr = readdir(dp)) != NULL; ++count)
    {
        if (all_content)
        {
            ptr[ j++ ] = (long) dptr -> d_name;
            if (strlen(dptr->d_name) > max_size)
            {
                max_size = strlen( dptr -> d_name );
            }
        }
        else
        {
            if (dptr->d_name[ 0 ] != '.')
            {
                ptr[ j++ ] = (long) dptr->d_name;
                if (strlen(dptr->d_name) > max_size)
                {
                    max_size = strlen(dptr->d_name);
                }
            }
        }
    }

    for (count = 0; count < num_files - 1; count++)
    {
        for (j = (count + 1); j < num_files; j++)
        {
            char *c = (char *) ptr[ count ];
            char *d = (char *) ptr[ j ];

            if ((*c >= 'a' && *d >= 'a') ||
                (*c <= 'Z' && *d <= 'Z'))
            {
                unsigned int i = 0;
                if (*c == *d)
                {
                    while (*(c + i) == *(d + i)) i++;
                }

                if (*(c + i) > *(d + i))
                {
                    long temp    = 0;
                    temp         = ptr[ count ];
                    ptr[ count ] = ptr[ j ];
                    ptr[ j ]     = temp;
                }
            }
        }
    }

    for (count = 0; count < num_files; count++)
    {
        if (count % 4 == 0 && count != 0) puts("");

        if (!access((const char *) ptr[ count ], X_OK))
        {
            struct stat st;
            int fd = -1;

            fd = open((char *) ptr[count], O_RDONLY, 0);
            if (fd == -1)
            {
                printf("\n Opening %s failed\n", (char *) ptr[ count ]);
                free_mem(ptr);
                exit(EXIT_FAILURE);
            }

            fstat(fd, &st);
            /*
             *  If a folder, paint it blue, if an executable, paint it green.
             */
            style(BOLD, ( S_ISDIR(st.st_mode) ? BLUE : GREEN ) );
            close(fd);
        }

        printf("%-*s", max_size + 1, (char *) ptr[ count ]);
        reset();
    }

    puts("");
    free_mem(ptr);

    return 1;
}
