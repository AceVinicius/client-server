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
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../lib/include/allocation.h"
#include "../../lib/include/list.h"
#include "../../lib/include/execute.h"
#include "../../lib/include/color.h"
#include "../../lib/include/ls.h"
#include "../../lib/include/directory.h"
#include "../../lib/include/general.h"



int
ls( LIST *command )
{
    bool all_content = false;

    OPTION *aux = command->options;
    for (size_t i = 0; aux != NULL; ++i, aux = aux->next)
    {
        if (!strcmp(aux->option, "-a"))
        {
            all_content = true;
        }
        ;
    }

    char curr_dir[ CWD_LIMIT ];
    if (getcwd(curr_dir, CWD_LIMIT) == NULL)
    {
        fprintf(stderr, "bash: couldn't get current directory\n");
        exit(EXIT_FAILURE);
    }

    struct dirent  *dptr = NULL;
    DIR            *dp   = open_dir(curr_dir);
    char          **ptr  = (char **) allocate(128, sizeof(char *));

    size_t num_files = 0;
    size_t max_size  = 0;

    while ((dptr = readdir(dp)) != NULL)
    {
        if (all_content ? true : (dptr->d_name[ 0 ]) != '.')
        {
            size_t size = strlen(dptr->d_name);

            ptr[ num_files ] = (char *) allocate(size, sizeof(char));
            ptr[ num_files++ ] = dptr->d_name;

            if (size > max_size)
            {
                max_size = size;
            }
        }
    }

    if (num_files == 0)
    {
        return 2;
    }


    // dp = reopen_dir(dp, curr_dir);
    

    // for (size_t count = 0, j = 0; (dptr = readdir(dp)) != NULL; ++count)
    // {
    //     if (all_content ? true : (dptr->d_name[ 0 ]) != '.')
    //     {
            
    //     }
    // }

    // close_dir(dp);

    // for (size_t count = 0; count < num_files - 1; count++)
    // {
    //     for (j = (count + 1); j < num_files; j++)
    //     {
    //         char *c = (char *) ptr[ count ];
    //         char *d = (char *) ptr[ j ];

    //         if ((*c >= 'a' && *d >= 'a') ||
    //             (*c <= 'Z' && *d <= 'Z'))
    //         {
    //             unsigned int i = 0;
    //             if (*c == *d)
    //             {
    //                 while (*(c + i) == *(d + i)) i++;
    //             }

    //             if (*(c + i) > *(d + i))
    //             {
    //                 long temp    = 0;
    //                 temp         = ptr[ count ];
    //                 ptr[ count ] = ptr[ j ];
    //                 ptr[ j ]     = temp;
    //             }
    //         }
    //     }
    // }

    // for (size_t count = 0; count < num_files; count++)
    // {
    //     if (count % 4 == 0 && count != 0) puts("");

    //     if (!access((const char *) ptr[ count ], X_OK))
    //     {
    //         struct stat st;
    //         int fd = -1;

    //         fd = open((char *) ptr[count], O_RDONLY, 0);
    //         if (fd == -1)
    //         {
    //             printf("\n Opening %s failed\n", (char *) ptr[ count ]);
    //             free_mem(ptr);
    //             exit(EXIT_FAILURE);
    //         }

    //         fstat(fd, &st);
    //         /*
    //          *  If a folder, paint it blue, if an executable, paint it green.
    //          */
    //         style(BOLD, ( S_ISDIR(st.st_mode) ? BLUE : GREEN ) );
    //         close(fd);
    //     }

    //     printf("%-s  ", (char *) ptr[ count ]);
    //     reset();
    // }

    // puts("");
    // free_mem(ptr);

    return 1;
}
