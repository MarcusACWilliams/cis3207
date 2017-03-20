


#include "myshell.h"

//int my_quit(char **args);



int my_clr()
{
	printf("\n");
	system("clear");
	return 1;
}


int my_quit(char **args)
{
	printf("Quitting Shell\n");
 	return 0;

}

int my_cd(char **args)
{
	if(args[1] != NULL)
	{
		if(chdir(args[1]) == 0)
		{
		printf("CD Succesful\n");
		free(enviorment.DIR);
		enviorment.DIR = getcwd(NULL, 0);
		}else{printf("Could not find directory %s\n", args[1]);}
	}
	return 1;
}

int my_echo(char **args)
{
	int i = 1;
	while(i < enviorment.arg_count)
	{	
	printf("%s ", args[i]);
	i++;
	}

	return 1;
}

int my_environ()
{

	printf("PATH : %s\n", getenv("PATH"));
   	printf("HOME : %s\n", getenv("HOME"));
   	printf("ROOT : %s\n", getenv("ROOT"));

	return 1;
}

int my_pause()// makes program hold untill enter is pressed
{
	getchar();
	return 1;
}

int my_help(char **args)
{
	char buffer[1024];
    char *more = "more -d ";
    char *echo = "echo ";
    char *slash = "/";
    char *n = "readme";

    FILE *fp;
    int c;

     
        strcpy(buffer, more);
        strcat(buffer, enviorment.DIR);
        strcat(buffer, slash);
        strcat(buffer, n);
        system(buffer);
    

    return 1;
}
