/***************************************************************
**Marcus Williams
**CIS 3207 Intro to Operating Systems
**Lab 2A
**Professor Dr. Eugene Kwatny
**February 16 2017

***********************Shell Outline****************************



***************************************************************/

 
#include <stdio.h>
#include <stdlib.h>

//add more header files as needed

//#include <my_shell.h>		//This file will contain all my function declaratons 



int batch = 0;              //Batch File Status Variable





int main(int argc, char **argv) 
{
 
	if (batch) {
	/*
	Take input from batch instead of user
	batch file name should be stored in argv[1]????
	*/
	}



  //Config Shell

	init_myshell(argv[1]);			//Use second argument to either select a batch file or 
									//some other confi command 



  //Run Command Loop

	myshell_cmd_loop();




  //Preform Exit Routine

	//free(/*allocated memory blocks go here*/);


	return EXIT_SUCCESS;


}

void init_myshell(void)				//Shell Configuration function
{

}


void myshell_cmd_loop(void)			//Command line loop starts here
{
	int status;


	do {
		printf("> ");

		//Read command line


		//Parse command line


		//Execute command that was entered 
	
	
	} while (status);
 
}

/*
cmd.c

This file will be accompanied by a header file, shell.h, and will include the 
code for the functions declared in the shell header file.

#include "myshell.h"  //???? 


int my_cd(char **argv)
{
	

}

void my_clr()
{

}

int my_dir(char **argv)
{

}

int my_exit(char **argv)
{

}

int my_environ(char **argv)
{

}

int my_echo(char **argv)
{

}

int my_help(char **argv)
{

}

int my_pause()
{

}

int my_quit()
{

}


*/