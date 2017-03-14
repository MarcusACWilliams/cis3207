#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

void init_myshell(char *arg);
void myshell_cmd_loop(void);
void parse_this(char *str,char **stra, int *c, char *in, char *out);
int shell_exe(char **stra);
void redi(char *in, char *out);
struct env_vars enviorment;



        const char *built_in[] = {"clear", "exit", "cd", "echo", "environ", "pause", "help"};

	int (*cmds[])(char **) = 
	{

	&my_clr,
	&my_quit,
	&my_cd,
	&my_echo,
	&my_environ,
	&my_pause,
	&my_help

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
	int status = 1, /*count = 0,*/ redirect = 0;
	char *line = malloc(1024);
	char *new_input = malloc(256);
	char *new_output = malloc(256);
	char **args = malloc(1024);


	//char *tok, *cmpr;

	do
	{
	printf("%s>", enviorment.DIR);

	fgets(line, 1024, stdin);

	parse_this(line,args, &redirect, new_input, new_output);// Parse user input
	if(redirect)
	{
	 redi(new_input, new_output);

	}
	

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

void parse_this(char *str,char **stra, int *flag, char *in, char *out)
{
	//char **tokens = malloc(1024);
	char *token ;
	int i = 0;	
	
	token = strtok(str," \n");

	while(token != NULL)
	{
	  stra[i] = token;

	//Detects the redirect characters in input stream and assigns file
	  if(flag == -1){in = token; *flag =2;}
	  else if(flag == 1){out = token; *flag = 3;}

	  token = strtok(NULL, " \n");
	  if(!strcmp(stra[i],"<"))
	  {
		*flag = -1;
		 printf("!!!!redirect!!!!");		
	  }
	   else if(!strcmp(stra[i],">"))
	   {
		*flag = 1; 
	   }

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
	 

	if(stra[0] == NULL || stra[0] == "\r")
	{
	printf("Please Enter a Command\n");
	return 1;
	}

	while(i < 8 && flag)
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

void redi(char *in, char *out)
{
	int in_fd, out_fd;

	if(in != NULL)
	{
	in_fd = open(in, O_RDONLY);
	dup2(in_fd, 0);	
	}

}
