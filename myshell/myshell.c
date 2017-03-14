#include <stdio.h>
#include <stdlib.h>
#include "myshell.h"

void init_myshell(char *argv);
void myshell_cmd_loop(void);
void parse_this(char *str,char **stra, int *c);
//int my_clr();
int shell_exe(char **stra);



        const char *built_in[] = {"clear", "exit"};

	int (*cmds[])(char **) = 
	{

	&my_clr,
	&my_quit

	};


int main(int argc, char **argv)
{

//	if(batch){
	

//	}


	init_myshell(argv[1]);


	myshell_cmd_loop();

	return 1;

}

void init_myshell(char *argv)
{

	return ;
}



void myshell_cmd_loop(void)
{
	int status = 1,j =0, count = 0;
	char *line = malloc(1024);
	char **args = malloc(1024);
	char *tok, *cmpr;

	do
	{
	printf(">");

	fgets(line, 1024, stdin);

	parse_this(line,args, &count);

	cmpr = args[0];

/*	if(!strcmp(tok,"clear"))
	{
	printf("\nCompare succesful!!");
	 cmds[0](args);
	}
*/	

	status = shell_exe(args);

	
	
	j=0;
	 tok = args[j];
 	           
/*	while(j < count && (tok != NULL))
	{
	 printf("\n%s",tok);
 	 j++;
	 tok = args[j];
	
	} 
*/
	}while(status);


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
/*
	printf("\n%d",*count); 
	*count = i;
	printf("\n%d\n",*count);

	 i = 0; 
*/
	/*// Test Block that prints out all the tokens of the parsed line
        token = stra[i];
	
	while(i < *count)
	{
	 printf("\n%s",token);
	 i++;
	 token = stra[i];
	 
	}*/
 

	printf("Line parse successful!\n");	
	
}

int shell_exe(char **stra)
{
	int i = 0, flag = 1;
	 

	printf("Enter execute\n");

	if(stra[0] == NULL)
	{
	printf("Nothing to execute\n");
	return 1;
	}

	while(i < 2 && flag)
	{
	
	 if(!strcmp(stra[0], built_in[i]))
	 {
	  flag = 0;
	  return (*cmds[i])(stra);
	 }

	 i++;
	}

	return 1;

}
