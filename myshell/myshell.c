#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

void init_myshell(char *arg);
void myshell_cmd_loop(void);
void parse_this(char *str,char **stra, int *c);
int shell_exe(char **stra);
struct env_vars enviorment;



        const char *built_in[] = {"clear", "exit", "cd", "echo", "environ"};

	int (*cmds[])(char **) = 
	{

	&my_clr,
	&my_quit,
	&my_cd,
	&my_echo,
	&my_environ

	};


int main(int argc, char **argv)
{
	char cwd[1024];

//	if(batch){
	

//	}


	init_myshell(cwd);


	myshell_cmd_loop();

	return 1;

}

void init_myshell(char *argv)
{
	
	if((enviorment.PWD = getcwd(NULL, 0) )!= NULL)
	{
	enviorment.DIR = enviorment.PWD;
	}else{perror("Error getting current directory\n");}

	return ;
}



void myshell_cmd_loop(void)
{
	int status = 1, count = 0;
	char *line = malloc(1024);
	char **args = malloc(1024);

	//char *tok, *cmpr;

	do
	{
	printf("%s>", enviorment.DIR);

	fgets(line, 1024, stdin);

	parse_this(line,args, &count);// Parse user input
	

	status = shell_exe(args); // Execute commands

	
	

//********Uncomment to print out all parsed tokens*********//
/*	int j = 0;	 
	tok = args[j]; 	           
	while(j < count && (tok != NULL))
	{
	 printf("\n%s",tok);
 	 j++;
	 tok = args[j];
	
	} 
*/

	}while(status);// Run cmd_loop untill status is set to 0


	return ;
}

void parse_this(char *str,char **stra, int *count)
{
	//char **tokens = malloc(1024);
	char *token ;
	int i = 0;	
	
	token = strtok(str," \n");

	while(token != NULL)
	{
	  stra[i] = token;
	  token = strtok(NULL, " ");
	  i++;
	}

	enviorment.arg_count = i;

/*	// Test Block that prints out all the tokens of the parsed line
	i = 0;
        token = stra[i];
	
	while(i < *count)
	{
	 printf("\n%s",token);
	 i++;
	 token = stra[i];
	 
	}*/
 
//	printf("Line parse successful!\n");	
	
}

int shell_exe(char **stra)
{
	int i = 0, flag = 1;
	 

//	printf("Enter execute\n");

	if(stra[0] == NULL)
	{
	printf("Please Enter a Command\n");
	return 1;
	}

	while(i < 5 && flag)
	{
	
	 if(!strcmp(stra[0], built_in[i]))
	 {
	  flag = 0;
	  return (*cmds[i])(stra);
	 }

	 i++;
	}
	
	printf("Command \"%s\" not found\n", stra[0]);

	return 1;

}