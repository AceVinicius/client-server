/**
 * @file execute.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-02-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../lib/include/execute.h"
#include "../../lib/include/history.h"



// TODO: Rewrite this function with a better approach
int
execute( char **command ,
         char **options )
{
    if (command[ 0 ] == NULL)
    {
        return CONTINUE_CLIENT;
    }
    else if (!strcmp(command[ 0 ], "exit"))
    {
        return EXIT_CLIENT;
    }
    else
    {
    // 	if (!strcmp(command[ 0 ], "cd"))
    // 	{
    // 		return cd(command,
    // 			      options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "ls"))
    // 	{
    // 		return ls(command,
    // 			      options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "wc"))
    // 	{
    // 		return wc(command,
    // 			      options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "mv"))
    // 	{
    // 		return mv(command,
    // 			      options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "rm"))
    // 	{
    // 		return rm(command,
    // 			      options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "cp"))
    // 	{
    // 		return cp(command,
    // 			      options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "cat"))
    // 	{
    // 		return cat(command,
    // 			       options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "pwd"))
    // 	{
    // 		const int var = pwd(options);
    // 		puts("");
    // 		return var;
    // 	}
    // 	if (!strcmp(command[ 0 ], "tail"))
    // 	{
    // 		return tail(command,
    // 			        options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "diff"))
    // 	{
    // 		return diff(command,
    // 			        options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "help"))
    // 	{
    // 		return help(command,
    // 			        options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "echo"))
    // 	{
    // 		return echo(command,
    // 			        options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "head"))
    // 	{
    // 		return head(command,
    // 			        options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "mkdir"))
    // 	{
    // 		return mk_dir(command,
    // 			          options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "rmdir"))
    // 	{
    // 		return rm_dir(command,
    // 			          options);
    // 	}
    // 	if (!strcmp(command[ 0 ], "touch")) {
    // 		return touch(command,
    // 			         options,
    // 					 direction);
    // 	}
        if (!strcmp(command[ 0 ], "history"))
        {
            if (history() == EXIT_FAILURE)
            {
                // TODO: Do something if an error occurs
            }

            return CONTINUE_CLIENT;
        }
    }
    
    fprintf(stderr, "client: command not found: %s\n", command[ 0 ]);

    return CONTINUE_CLIENT;
}
