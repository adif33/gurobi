#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "game.h"

int do_command(CMD* command){
	char *endptr;
	int x =0 ;
	int y =0 ;
	int z =0;


	switch (command->type) {
	case SET:
	    printf("set cmd\n");

	    printf("param 0 : %s \n", (&(command->args))->param0);
	    x = strtol((&(command->args))->param0,&endptr, 10);
	    if (endptr == (&(command->args))->param0 )
	     {
	     	printf("ERROR: Parameter Wrong conversion\n");
	     	return 0;
	     } 

	    printf("param 1 : %s \n", (&(command->args))->param1);

	   	y = strtol((&(command->args))->param1,&endptr, 10);
	    if (endptr == (&(command->args))->param1 )
	     {
	     	printf("ERROR: Parameter Wrong conversion\n");
	     	return 0;
	     } 
	    printf("param 2 : %s \n", (&(command->args))->param2);
	    z = strtol((&(command->args))->param2,&endptr, 10);
	    if (endptr == (&(command->args))->param2 )
	     {
	     	printf("ERROR: Parameter Wrong conversion\n");
	     	return 0;
	     } 

	    printBoard();
	    break;
    }

}




int play_game(){
	
	int n = 0;
	int m =0;
	int is_over = 0;
	printf("play game\n");
	get_n_m(&n,&m);
	printBoard();
	

	while(1){
		CMD cmd = {0};
		char cmd_text[CMD_MAX_LENGTH + 2] = {0};

		is_over = get_command(cmd_text);

		/*
		printf("fgets returned : %i \n", is_over);
		*/
		if (is_over ==1 )
		{
			break;
		}

		if (strlen(cmd_text) > CMD_MAX_LENGTH )
		{
			printf("ERROR: CMD too long\n");
			continue;
		}

		if ( !parse_command(cmd_text,&cmd) )
		{
			continue;
		} 
		if (!do_command(&cmd)) {
			continue;
		}

	}

	return is_over;

}
