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
	} else if (strcmp(type, "solve") == 0){
		command->type = SOLVE;
		argsNum = 1;
	} else if (strcmp(type, "edit") == 0){
		command->type = EDIT;
		argsNum = 1;
	} else if (strcmp(type, "mark_errors") == 0){
		command->type = MARK_ERRORS;
		argsNum = 1;
	} else if (strcmp(type, "print_board") == 0){
		command->type = PRINT_BOARD;
		argsNum = 0;
	} else if (strcmp(type, "set") == 0) {
        command->type = SET;
        argsNum = 3;
    } else if (strcmp(type, "validate") == 0) {
        command->type = VALIDATE;
        argsNum = 0;
    } else if (strcmp(type, "guess") == 0) {
        command->type = GUESS;
        argsNum = 1;
    } else if (strcmp(type, "generate") == 0) {
        command->type = GENERATE;
        argsNum = 2;
    } else if (strcmp(type, "undo") == 0) {
        command->type = UNDO;
        argsNum = 0;
    } else if (strcmp(type, "redo") == 0) {
        command->type = REDO;
        argsNum = 0;
    } else if (strcmp(type, "save") == 0) {
        command->type = SAVE;
        argsNum = 1;
    } else if (strcmp(type, "hint") == 0) {
        command->type = HINT;
        argsNum = 2;
    } else if (strcmp(type, "guess_hint") == 0) {
        command->type = GUESS_HINT;
        argsNum = 2;
    } else if (strcmp(type, "num_solutions") == 0) {
        command->type = NUM_SOLUTIONS;
        argsNum = 0;
    } else if (strcmp(type, "autofill") == 0) {
        command->type = AUTOFILL;
        argsNum = 0;
    } else if (strcmp(type, "reset") == 0) {
        command->type = RESET;
        argsNum = 0;
    } else if (strcmp(type, "exit") == 0) {
        command->type = EXIT;
        argsNum = 0;
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
				(&(command->args))->x = par;
				par = strtok(NULL," \n"); 
				i++;
				continue; 
			case 1 :
				(&(command->args))->y = par;
				par = strtok(NULL," \n"); 
				i++;
				continue; 
			case 2 :
				(&(command->args))->z = par;
				par = strtok(NULL," \n"); 
				i++;
				continue; 
			case 3:
				i++;
				break;
		}

    }
    /*
    printf("type is : %s\n",type );
    printf("i is : %i \n",i );
    printf("args mun is : %i\n",argsNum);
    */
    if (i != argsNum)
	{
		if (command->type == EDIT && i ==0 )
		{
			return 1;
		} else if (i < argsNum)
		{
			switch (i)
			{
				case 0 :
				printf("Error: paran x is missing\n");
				break;
				case 1 :
				printf("Error: paran y is missing\n");
				break;
				case 2 :
				printf("Error: paran z is missing\n");
			}
		} else {
			printf("Error: too many arguments\n");
		}
		return 0;

	}
	return 1;
}

