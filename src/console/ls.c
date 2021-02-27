// # include "../lib/headers/include.h"
// # include "../lib/headers/define.h"
// # include "../lib/headers/global_variables.h"
// # include "../lib/headers/style.h"
// # include "../lib/headers/command.h"
// # include "../lib/headers/input.h"



// int
// ls(char **command,
//    char **options,
//    char **direction)
// {
// 	unsigned int num_files = 0;
// 	unsigned int max_size  = 0;
// 	unsigned int count     = 0;

// 	bool all_content = false;
// 	// bool file_print  = false;

// 	struct dirent *dptr = NULL;

// 	long *ptr = NULL;

// 	char curr_dir[ PWD_LIMIT ];

// 	DIR *dp = NULL;



// 	for (register unsigned int i = 0; options[ i ] != NULL; ++i) {
// 		if (!strcmp(options[ i ], "-a")) {
// 			all_content = true;
// 		}
// 	}

// 	// for (register unsigned int i = 0; direction[ i ] != NULL; ++i) {
// 	// 	if (command[ i ] == ">") {
// 	// 		file_print = true;
// 	// 	}
// 	// }



// 	if (getcwd(curr_dir, sizeof(curr_dir)) == NULL) {
// 		fprintf(stderr, "bash: couldn't get current directory\n");

// 		free(ptr);

// 		exit(EXIT_FAILURE);
// 	}



// 	dp = opendir((const char*) curr_dir);

// 	if (dp == NULL) {
// 		fprintf(stderr, "bash: Could not open the working directory\n");

// 		free(ptr);

// 		exit(EXIT_FAILURE);
// 	}

// 	while ((dptr = readdir(dp)) != NULL) {
// 		if (all_content) {
// 			num_files++;
// 		} else {
// 			if ((dptr -> d_name[ 0 ]) != '.') {
// 				num_files++;
// 			}
// 		}
// 	}

// 	closedir(dp);

// 	dptr = NULL;
// 	dp   = NULL;

// 	if (!num_files) {
// 		return 2;
// 	} else {
// 		ptr = (long *) malloc(sizeof(void *) * num_files);

// 		if (!ptr) {
// 			fprintf(stderr, "bash: 'ptr' allocation error\n");

// 			free(ptr);

// 			exit(EXIT_FAILURE);
// 		} else {
// 			memset(ptr, 0, sizeof(void *) * num_files);
// 		}
// 	}



// 	dp = opendir((const char *) curr_dir);

// 	if (dp == NULL) {
// 		fprintf(stderr, "bash: Could not open the working directory\n");

// 		free( ptr );

// 		exit(EXIT_FAILURE);
// 	}

// 	unsigned int j = 0;

// 	for (count = 0; (dptr = readdir(dp)) != NULL; ++count) {
// 		if (all_content) {
// 			ptr[ j++ ] = (long) dptr -> d_name;
// 		} else {
// 			if (dptr -> d_name[ 0 ] != '.') {
// 				ptr[ j++ ] = (long) dptr -> d_name;
// 			}
// 		}

// 		if (strlen( dptr -> d_name ) > max_size) {
// 			max_size = strlen( dptr -> d_name );
// 		}
// 	}


// 	for (count = 0; count < num_files - 1; count++) {
// 		for (j = (count + 1); j < num_files; j++) {
// 			char *c = (char *) ptr[ count ];
// 			char *d = (char *) ptr[ j ];

// 			if ((*c >= 'a' && *d >= 'a') ||
// 			    (*c <= 'Z' && *d <= 'Z')) {

// 				unsigned int i = 0;

// 				if (*c == *d) {
// 					while (*(c + i) == *(d + i)) {
// 						i++;
// 					}
// 				}

// 				if (*(c + i) > *(d + i)) {
// 					long temp    = 0;

// 					temp         = ptr[ count ];
// 					ptr[ count ] = ptr[ j ];
// 					ptr[ j ]     = temp;
// 				}
// 			}
// 		}
// 	}



// 	for (count = 0; count < num_files; count++) {
// 		if (!access((const char *) ptr[ count ], X_OK)) {
// 			struct stat st;

// 			int fd = -1;

// 			fd = open((char *) ptr[count], O_RDONLY, 0);

// 			if (fd == -1) {
// 				printf("\n Opening file/Directory failed\n");

// 				free(ptr);

// 				exit(EXIT_FAILURE);
// 			}

// 			fstat(fd, &st);

// 			if (count % 2 == 0 &&
// 			    count != 0) {
// 				puts("");
// 			}

// 			/*
// 			 *  If a folder, paint it blue, if an executable, paint it green.
// 			 */
// 			if (S_ISDIR(st.st_mode)) {
// 				style(BOLD, BLUE);
// 					printf("%-*s", max_size + 3, (char *) ptr[ count ]);
// 				reset( );
// 			} else {
// 				style(BOLD, GREEN);
// 					printf("%-*s", max_size + 3, (char *) ptr[ count ]);
// 				reset( );
// 			}

// 			close(fd);
// 		} else {
// 			if (count != 0 && 
// 				count % 2 == 0) {
// 				puts("");
// 			}

// 			printf("%-*s", max_size + 3, (char *) ptr[ count ]);
// 		}
// 	}

// 	puts("");
// 	free(ptr);

// 	return 1;
// }
