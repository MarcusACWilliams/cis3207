


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
