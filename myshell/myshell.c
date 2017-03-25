#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

void init_myshell(char *arg);
void myshell_cmd_loop(void);
int parse_this(char *str,char **stra, int *redirect, int *pipe_flag, int *bg , int *in, int *out);
int shell_exe(char **stra, int *redirect, int *pipe_flag, int *new_in, int *new_out);
void redi(char **stra, int *new_in, int *new_out);
int pipe_to(int *pipe_flag, char **stra);
int run(char **stra, int *redirect, int *pipe_flag,int *new_in, int *new_out);
int background();

 env_vars enviorment;



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
	int status = 1,redi_flag = 0, *redirect = &redi_flag, p_flag = 0, *pipe_flag = &p_flag,bgint = 0, *bg = &bgint;
	char *line = malloc(1024);
	int in = 0,  *new_input = &in;
	int out = 0, *new_output = &out;
	char **args = malloc(1024);


	do
	{
	printf("%s>", enviorment.DIR);


	fgets(line, 1024, stdin);

	parse_this(line, args, redirect, pipe_flag, bg, new_input, new_output);// Parse user input	

	status = shell_exe(args, redirect, pipe_flag, new_input, new_output); // Execute commands

//********Uncomment to print out all parsed tokens*********//
	// char *tok;
	// int j = 0;	 
	// tok = args[j]; 	           
	// while(tok != NULL)
	// {
	//  printf("\n%s",tok);
    // 	 j++;
	//  tok = args[j];
	
	// } 

	memset(line, 0, 1024);
	memset(args, 0, 1024);

	}while(status);// Run cmd_loop untill status is set to 0


	return ;
}

int parse_this(char *str,char **stra, int *redirect, int *pipe_flag, int*bg, int *new_in, int *new_out)
{
	char *token ;
	int i = 0;	

	for(token = strtok(str, "  \n"); token != NULL; token = strtok(NULL, " \n"))
	{
	  stra[i] = token;

	  if(!strcmp(stra[i],"<"))
	  {
		 *new_in = i+1;
		 stra[i] = NULL;
		 *redirect = 1;		
	  }
	   else if(!strcmp(stra[i],">"))
	   {
		 *new_out = i+1; 
		 stra[i] = NULL;
		 *redirect = 1;
	   }

	  if(!strcmp(stra[i], "|"))
	  {
	  	*pipe_flag = i;
	  }

	  // if(!strcmp(stra[i], "&"))
	  // {
	  // 	*bg = 1;
	  // }

	  i++;
	}

	enviorment.arg_count = i;
	return 1;
	
}

// Check user input against the list of built in commands and if not attempt to run as an external program
int shell_exe(char **stra, int *redirect, int *pipe_flag, int *new_in, int *new_out)
{
	int i = 0, j = 0, flag = 1;
	//char* word;
	// int *in = *&new_in, *out = *&new_out;
	// char **arg = *&stra;
	//char pro_1 = malloc(256), pro_2 = malloc(256);
	 
	if(stra[0] == NULL || stra[0] == "\r")// Check for empty input string
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
	if(!run(stra, redirect, pipe_flag ,new_in, new_out))
	{
		printf("Command \"%s\" not found\n", stra[0]);
	}
	

	return 1;

}

// Invoked whenever the redirection flag is set to TRUE
void redi(char **stra, int *new_in, int *new_out)
{
	int in_fd, out_fd;

	if(new_in != 0)
	{
		if( (in_fd = open(stra[*new_in], O_RDONLY)) == -1) // Open input file and store its file descriptor in in_fd
		{
			perror("Error opening file");

		}else
			{
			dup2(in_fd, 0);				// Next change input file's file descriptor to 0 effectively making it the new stdin
			}							// **For future referencing, if it still seems counter intuitive, remeber that dup2(old,new) is makes "old" and "new" interchangeable file
		close(in_fd);					// 		descriptors. If "new" is already in use (i.e. stdin) it is atomicly closed, which is the main advantage of dup2 over dup
	}								
	if(new_out != 0)				
	{							 
	out_fd = open(stra[*new_out], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWGRP | S_IWUSR); // Same as above but now you are redirecting stdout to a file 
	dup2(out_fd, 1);	
	close(out_fd);			
	}


}

// If a pipe between two child processes is needed use this function
int pipe_to(int *pipe_flag, char **stra)
{
	int pipe_fd[2];
	pid_t head, tail;// pid;
	int status, n = 2;
	//char buf;


	if(pipe(pipe_fd) == -1)
	{
		perror("Error creating pipe");
		return 1;
	}

	head = fork();
	
	if(head == 0)
	{
		dup2(pipe_fd[1],1);
		close(pipe_fd[1]);
		execvp(stra[*pipe_flag-1], stra);
	}

	tail = fork();

	if(tail == 0)
	{
		dup2(pipe_fd[0],0);
		close(pipe_fd[0]);
		execvp(stra[*pipe_flag+1], stra);
	}

	close(pipe_fd[0]);
	close(pipe_fd[1]);

	while (n > 0) {
  	wait(&status);
  	//printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
  	--n;  // TODO(pts): Remove pid from the pids array.
    }
	*pipe_flag = 0;

	return 1;
}




int run(char **stra,int *redirect, int *pipe_flag, int *new_in, int *new_out)
{
	int status;
	pid_t childPID;

	if(*pipe_flag)
	{

		pipe_to(pipe_flag, stra);

	}
	else if((childPID = fork()) == -1)
	{
		perror("Error Starting Program");
		return 0;
	}
	else if(childPID == 0)// Do in child process
	{
		if(*redirect)
		{
	 	redi(stra, new_in, new_out);
	 	*redirect = 0;
		}

		if(execvp(stra[0], stra) == -1)
		{
		perror("Error Running Program");
		return 0;
		}
		// if(bg)
		// {
		// background();
		// }
	}else
	{
		wait(&status);
		*redirect = 0;

	}

	return 1;

} 

int background()
{
	printf("Setting program to background\n");
	setpgid(0,0);
	return 1;
}
