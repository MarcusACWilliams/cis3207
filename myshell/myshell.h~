#ifndef MYSHELL_H_
#define MYSHELL_H_


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_cd(char**);
int my_clr();
int my_quit(char**);

extern const char *built_in[];
extern int (*cmds[])(char **);

struct env_vars
{
	char *PWD;
	char *DIR;


};

extern struct env_vars enviorment;

#endif
