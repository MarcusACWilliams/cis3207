#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

void init_myshell(char *arg);
void myshell_cmd_loop(void);
void parse_this(char *str,char **stra, int *flag, int *p_flag, int *bg , char *in, char *out);
int shell_exe(char **stra, int *bg);
void redi(char *new_in, char *new_out);
int pipe_to(int p1, int p2, char **stra);
int run(char **stra, int *bg);
int background();
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
	int status = 1, /*count = 0,*/ redirect = 0, p_flag = 0, bg = 0;
	char *line = malloc(1024);
	char *new_input = malloc(256);
	char *new_output = malloc(256);
	char **args = malloc(1024);


	do
	{
	printf("%s>", enviorment.DIR);

	fgets(line, 1024, stdin);

	parse_this(line, args, &redirect, &p_flag, &bg, new_input, new_output);// Parse user input
	if(redirect)
	{
	 redi(new_input, new_output);

	}
	if(pipe)
	{

	}
	if(bg)
	{

	}	

	status = shell_exe(args, bg); // Execute commands

	
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

void parse_this(char *str,char **stra, int *flag, int *p_flag, int*bg, char *new_in, char *new_out)
{
	//char **tokens = malloc(1024);
	char *token ;
	int i = 0;	
	
	token = strtok(str," \n");

	while(token != NULL)
	{
	  stra[i] = token;

	//Detects the redirect characters in input stream and assigns file
	  if(*flag == -1){new_in = token; *flag =2;}
	  else if(*flag == 1){new_out = token; *flag = 3;}

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

	  if(!strcmp(stra[i], "|"))
	  {
	  	*p_flag = 1;
	  	pipe_to(i, i+2, stra);
	  }

	  if(!strcmp(stra[i], "&"))
	  {
	  	*bg = 1;
	  }

	  i++;
	}

	enviorment.arg_count = i;
	
}

// Check user input against the list of built in commands and if not attempt to run as an external program
int shell_exe(char **stra, int *bg)
{
	int i = 0, j = 0, flag = 1;
	char pro_1 = malloc(256), pro_2 = malloc(256);
	 

	if(stra[0] == NULL || stra[0] == "\r")
	{
	printf("Please Enter a Command\n");
	return 1;
	}

	while(i < 8 && flag)
	{
	
	 	if(!strcmp(stra[j], built_in[i]))
	 	{
	  	flag = 0;
	 	return (*cmds[i])(stra);
	 	}

	 	i++;
	}
	
	// Run arg[0] as a program if it fails print message
	if(run(stra, bg))
	{
		printf("Command \"%s\" not found\n", stra[0]);
	}
	
	

	return 1;

}

// Invoked whenever the redirection flag is set to TRUE
void redi(char *new_in, char *new_out)
{
	int in_fd, out_fd;

	if(new_in != NULL)
	{
		if( (in_fd = open(new_in, O_RDONLY)) == -1) // Open input file and store its file descriptor in in_fd
		{
			perror("Error opening file");

		}else
			{
			dup2(in_fd, 0);				// Next change input file's file descriptor to 0 effectively making it the new stdin
			}							// **For future referencing, if it still seems counter intuitive, remeber that dup2(old,new) is makes "old" and "new" interchangeable file
										// 		descriptors. If "new" is already in use (i.e. stdin) it is atomicly closed, which is the main advantage of dup2 over dup
	}								
	if(new_out != NULL)				
	{							 
	out_fd = open(new_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWGRP | S_IWUSR); // Same as above but now you are redirecting stdout to a file 
	dup2(out_fd, 1);				
	}
}

// If a pipe between two child processes is needed use this function
int pipe_to(int p1, int p2, char **stra)
{
	int pipe_fd[2];
	pid_t head, tail;
	//char buf;

	if(pipe(pipe_fd) == -1)
	{
		perror("Error creating pipe");
		return 1;
	}

	head = fork();
	
	if(head == 0)
	{
		close(pipe_fd[1]);
		execvp(stra[p1], stra);
	}

	tail = fork();

	if(tail == 0)
	{
		close(pipe_fd[0]);
		execvp(stra[p2], stra);
	}

	close(pipe_fd[0]);
	close(pipe_fd[1]);
}




int run(char **stra,int *bg)
{
	int status;
	pid_t childPID;
	
	if((childPID = fork()) == -1)
	{
		perror("Error Starting Program");	
	}
	else if(childPID == 0)// Do in child process
	{
		if(execvp(stra[0], stra) == -1)
		{
		perror("Error Running Program");
		exit(EXIT_FAILURE);
		}
		if(bg)
		{
		background();
		}
	}else
	{
		waitpid(childPID, &status, 0);
		return 0;	

	}

	return 1;

} 

int background()
{
	printf("Setting program to background\n");
	setpgid(0,0);
}
