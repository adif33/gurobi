#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "game.h"

int doSolveCommand(CMD* command){
	printf("solv cmd\n");
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}

int doEditCommand(CMD* command){
	printf("edit cmd\n");
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doMarkErrorsCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doValidateCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doGuessCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	printf("test\n");
	return 1;
}
int doGenerateCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	printf("param y : %s \n", (&(command->args))->y);
	return 1;
}
int doUndoCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doRedoCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doSaveCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doHintCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	printf("param y : %s \n", (&(command->args))->y);
	return 1;
}
int doGuessHintCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	printf("param y : %s \n", (&(command->args))->y);
	return 1;
}
int doNumSolutionsCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doAutofillCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doResetCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}
int doExitCommand(CMD* command){
	printf("param x : %s \n", (&(command->args))->x);
	return 1;
}

int doSetCommand(CMD* command){
	int x = 0;
	int y = 0;
	int z = 0;
	char *endptr;

	printf("param x : %s \n", (&(command->args))->x);
	x = strtol((&(command->args))->x,&endptr, 10);
	if (endptr == (&(command->args))->x )
	 {
	 	printf("Error: Parameter x Wrong conversion\n");
	 	return 0;
	 } 

	printf("param y : %s \n", (&(command->args))->y);

		y = strtol((&(command->args))->y,&endptr, 10);
	if (endptr == (&(command->args))->y )
	 {
	 	printf("Error: Parameter y Wrong conversion\n");
	 	return 0;
	 } 
	printf("param z : %s \n", (&(command->args))->z);
	z = strtol((&(command->args))->z,&endptr, 10);
	if (endptr == (&(command->args))->z )
	 {
	 	printf("Error: Parameter z Wrong conversion\n");
	 	return 0;
	 } 

}

int do_commands(CMD* command, Board* board){

	switch (command->type) {
	case SOLVE:
	    doSolveCommand(command);
	    printBoard(board);
	    break;

	case EDIT:
	    doEditCommand(command);
	    printBoard(board);
	    break;
	case MARK_ERRORS:
		printf("mark errors cmd\n");
	    if (doMarkErrorsCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case PRINT_BOARD:
	    printBoard(board);
	    break;

	case SET:
	    printf("set cmd\n");
	    if (doSetCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case VALIDATE:
	    printf("validate cmd\n");
	    if (doValidateCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case GUESS:
	    printf("guess cmd\n");
	    if (doGuessCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case GENERATE:
	    printf("generate cmd\n");
	    if (doGenerateCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case UNDO:
	    printf("undo cmd\n");
	    if (doUndoCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case REDO:
	    printf("redo cmd\n");
	    if (doRedoCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case SAVE:
	    printf("save cmd\n");
	    if (doSaveCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case HINT:
	    printf("hint cmd\n");
	    if (doHintCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case GUESS_HINT:
	    printf("guess hint cmd\n");
	    if (doGuessHintCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case NUM_SOLUTIONS:
	    printf("NumSolutions cmd\n");
	    if (doNumSolutionsCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case AUTOFILL:
	    printf("Autofill cmd\n");
	    if (doAutofillCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case RESET:
	    printf("reset cmd\n");
	    if (doResetCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;
	case EXIT:
	    printf("Exit cmd\n");
	    if (doExitCommand(command))
	    {
	    	printBoard(board);
	    }
	    break;


    }

}




int play_game(){
	
	int n = 0;
	int m =0;
	int is_over = 0;
	Board* board;
	board = createEmptyBoard();
	printf("play game\n");
	get_n_m(&n,&m);
	printBoard(board);
	

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
		if (!do_commands(&cmd, board)) {
			continue;
		}

	}

	return is_over;

}
