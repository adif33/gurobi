#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"


void get_n_m(int *n,int*m){
	
	int tm =0;
	int tn =0;
	int flag = 0;

	printf("Please enter n and m:\n");
	flag = scanf("%i %i", &tm,&tn);
	if (flag <= 0)
	{
		printf("Error in scanf()\n");
		exit(-1);
	}

	printf("M is: %i, N is: %i.\n",tm,tn );
}


int get_command(char* text_cmd){
	return fgets(text_cmd, CMD_MAX_LENGTH + 2, stdin) == NULL;
}

int parse_command(char* text, CMD* command){
	char* par;
	int i =0 ;
	int argsNum = 0;

	char* type ;
	type = strtok(text, " \t\r\n");
	if (type == NULL) {
		command->type = SKIP;
		return 1;
	} else if (strcmp(type, "set") == 0) {
        command->type = SET;
        argsNum = 3;
    } else if (strcmp(type, "validate") == 0) {
        command->type = VALIDATE;
    } else if (strcmp(type, "restart") == 0) {
        command->type = RESTART;
    } else if (strcmp(type, "exit") == 0) {
        command->type = EXIT;
    } else {
    	printf("invalid cmd\n");
    	return 0;

    }

    par = strtok(NULL," \n"); 
    while( par != NULL && i <= 3) {
    	/*
    	printf("par is: %s\n",par );
    	*/
		switch(i) {
			case 0 :
				(&(command->args))->param0 = par;
				par = strtok(NULL," \n"); 
				i++;
				continue; 
			case 1 :
				(&(command->args))->param1 = par;
				par = strtok(NULL," \n"); 
				i++;
				continue; 
			case 2 :
				(&(command->args))->param2 = par;
				par = strtok(NULL," \n"); 
				i++;
				continue; 
			case 3:
				i++;
				break;
		}

    }

    if (i != argsNum)
	{
		printf("Wrong number of arguments !\n");
		return 0;
	}
	return 1;
    
}

