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

#include "../../lib/include/history.h"
#include "../../lib/include/list.h"
#include "../../lib/include/execute.h"



static inline int  cmp           ( const char *str1 ,const char *str2 );
static int         read_command  ( const char * );



/**
 * @brief Campare two words to see if they are equal
 * 
 * @param str1 Word 1
 * @param str2 Word 2
 * @return int Returns 1 if they are equal and 0 if not
 */
static inline int
cmp( const char *str1 ,
     const char *str2 )
{
    return !strcmp(str1, str2);
}



/**
 * @brief Receive a word and returns an id
 * 
 * @param command Word to be identified
 * @return int ID of the command
 */
static int
read_command( const char *command )
{
    if (command == EMPTY_CMD)
    {
        return EMPTY_CMD_ID;
    }
    else if (cmp(command, EXIT_CMD))
    {
        return EXIT_CMD_ID;
    }
    else if (cmp(command, HISTORY_CMD))
    {
        return HISTORY_CMD_ID;
    }
    else if (cmp(command, STAT_CMD))
    {
        return STAT_CMD_ID;
    }
    else if (cmp(command, REMOVE_CMD))
    {
        return REMOVE_CMD_ID;
    }
    else if (cmp(command, SEND_CMD))
    {
        return SEND_CMD_ID;
    }
    else if (cmp(command, FETCH_CMD))
    {
        return FETCH_CMD_ID;
    }
    else if (cmp(command, LS_CMD))
    {
        return LS_CMD_ID;
    }
    else if (cmp(command, CD_CMD))
    {
        return CD_CMD_ID;
    }
    
    return UNKNOWN_CMD_ID;
}



/**
 * @brief Call the proper function based in the user input
 * 
 * @param command List of operation and parameters
 * @return int Returns true if the program will run again or 0 if user wants
 *             to terminate
 */
int
execute( LIST *command )
{
    const int cmd_id = read_command(command->command);
    int error = 0;
    
    switch (cmd_id)
    {
        case EMPTY_CMD_ID:
            break;
            
        case EXIT_CMD_ID:
            return EXIT_CLIENT;
            break;
            
        case HISTORY_CMD_ID:
            error = history();
            break;
        
        case STAT_CMD_ID:
            break;
            
        case REMOVE_CMD_ID:
            break;
        
        case SEND_CMD_ID:
            break;
            
        case FETCH_CMD_ID:
            break;
            
        case LS_CMD_ID:
            break;
            
        case CD_CMD_ID:
            break;
            
        default:
            fprintf(stderr, "client: command not found: %s\n", command->command);
            break;
    }

    if (error)
    {
        // TODO: Do something
    }

    return CONTINUE_CLIENT;
}
