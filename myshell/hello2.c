
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void main()
{
	char *line = malloc(64);

	fgets(line, 64, stdin);
	strcat(line, "---New output\n");
	fputs(line, stdout);
	//printf("Hello World\n");
	free(line);

	return;

}