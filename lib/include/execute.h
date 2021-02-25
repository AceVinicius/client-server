#ifndef EXECUTE_H
# define EXECUTE_H



# define EXIT_CLIENT        false
# define CONTINUE_CLIENT     true

# define UNKNOWN_CMD_ID        -1
# define EMPTY_CMD_ID           0
# define REMOVE_CMD_ID          1
# define SEND_CMD_ID            2
# define FETCH_CMD_ID           3
# define HISTORY_CMD_ID         4
# define STAT_CMD_ID            5
# define EXIT_CMD_ID            6
# define HELP_CMD_ID            7
# define LS_CMD_ID              8
# define CD_CMD_ID              9

# define EMPTY_CMD           NULL
# define REMOVE_CMD          "rm"
# define SEND_CMD          "send"
# define FETCH_CMD        "fetch"
# define HISTORY_CMD    "history"
# define STAT_CMD          "stat"
# define EXIT_CMD          "exit"
# define HELP_CMD          "help"
# define LS_CMD              "ls"
# define CD_CMD              "cd"



int  execute  ( LIST * );



#endif // EXECUTE_H
