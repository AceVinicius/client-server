// # include "../lib/headers/include.h"
// # include "../lib/headers/define.h"
// # include "../lib/headers/global_variables.h"
// # include "../lib/headers/style.h"
// # include "../lib/headers/command.h"
// # include "../lib/headers/input.h"



// int
// cd(char **command,
//    char **options,
//    char **direction)
// {
// 	if (command[ 1 ] == NULL)
// 	{
// 		unsigned int   uid = getuid();

// 		struct passwd *pwd = getpwuid(uid);
// 		if (!pwd)
// 		{
// 			fprintf(stderr, "bash: User with %u ID is unknown.\n", uid);
// 			return 2;
// 		}

// 		if (chdir(pwd -> pw_dir))
// 		{
// 			fprintf(stderr, "Unable to change directory to '%s'\n", pwd -> pw_dir);
// 			return 2;
// 		}
// 	}
// 	else
// 	{
// 		if (chdir(command[ 1 ]))
// 		{
// 			fprintf(stderr, "bash: cd: %s: No such file or directory\n", command[ 1 ]);
// 			return 2;
// 		}
// 	}
// 	return 1;
// }
