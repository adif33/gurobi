#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "game.h"
#include "main_aux.h"


int convertCommandToInt(CMD* command,int argsNum){
    int x = -1;
    int y = -1;
    int z = -1;
    char *endptr;

    x = strtol((&(command->args))->x,&endptr, 10);
    if (endptr == (&(command->args))->x )
    {
        printf(PARAMETER_X_CONVERSION_ERROR);
        return 0;
    }
    (&(command->args))->x_int = x;
    if (argsNum == 1){
        return 1;
    }

    y = strtol((&(command->args))->y,&endptr, 10);
    if (endptr == (&(command->args))->y )
    {
        printf(PARAMETER_Y_CONVERSION_ERROR);
        return 0;
    }
    (&(command->args))->y_int = y;
    if (argsNum == 2){
        return 1;
    }

    z = strtol((&(command->args))->z,&endptr, 10);
    if (endptr == (&(command->args))->z )
    {
        printf(PARAMETER_Z_CONVERSION_ERROR);
        return 0;
    }
    (&(command->args))->z_int = z;
    return 1;
}

/*DOUBLY LINKED LIST - UNDO REDO*/

void freeNextNodes(Node * node){
    Node * tmp ;
    Node * tmp1 ;
    tmp = node;
    while (tmp != NULL ){
        tmp1 = tmp->next;
        printf("freeing node %p , board %p \n",(void*)tmp,(void*)tmp->board);
        freeBoard(tmp->board);
        free(tmp);
        tmp = tmp1;
    }
}

void printList(DubList* list){
    Node * node ;
    Node* tmp;

    node = list->curr;
    if ( node == NULL){
        printf("list is empty \n");
    } else {
        printf("FOREWARD:\n");
        printf("-----------\n");
        tmp = node;
        while (tmp != NULL )
        {
            printf("node addr is  %p \n",(void*)tmp);
            printf("\tnext id is: %p  \n",(void*)tmp->next);
            printf("\tprev id is: %p  \n",(void*)tmp->prev);
            printf("\tboard is: %p  \n",(void*)tmp->board);
            tmp = tmp->next;
        }
        printf("BACKWARDS:\n");
        printf("-----------\n");
        tmp = node;
        while (tmp != NULL )
        {
            printf("node addr is  %p \n",(void*)tmp);
            printf("\tnext id is: %p  \n",(void*)tmp->next);
            printf("\tprev id is: %p  \n",(void*)tmp->prev);
            printf("\tboard is: %p  \n",(void*)tmp->board);
            tmp = tmp->prev;
        }
        printf("done printing \n");

    }
}

int pushToList(DubList* list, Board* boardPtr){

    Node* tmp;
    Node* current;
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->board = boardPtr;
    newNode->next = NULL;
    current = list->curr;

    if (current == NULL)
    {
        /*First element*/
        printf("first elem \n");
        list->curr = newNode;
        newNode->prev = NULL;
    }   else {
        /*Not First element		*/
        tmp = current->next;
        current->next = newNode;
        newNode->prev = current;
        /*
        printf("print after push: \n");
        printList(list);
        */
        list->curr = newNode;


        freeNextNodes(tmp);

    }

    return 0;
}


int goStepBack(DubList* list){
    Node * node ;
    node = list->curr;

    if (node == NULL ){
        printf("Error: list is empty\n");
        return 0;
    } else if (node->prev == NULL){
        printf("Error: no more undos \n");
        return 0;
    } else {
        list->curr = node->prev;
        return 1;
    }
}
int goStepForward(DubList* list){
    Node * node ;
    node = list->curr;

    if (node == NULL ){
        printf("Error: list is empty\n");
        return 0;
    } else if (node->next == NULL){
        printf("Error: no more Redos \n");
        return 0;
    } else {
        list->curr = node->next;
        return 1;
    }
}

int freeList(DubList* list){
    Node* node = list->curr;
    Node* tmp;

    if (node == NULL){
        return 1;
    }

    /*Go back all the way*/
    tmp = node->prev;

    while (tmp != NULL )
    {
        node = tmp;
        tmp = node->prev;

    }
    /*free all next nodes*/
    list->curr = NULL;
    freeNextNodes(node);
    return 1;
}

/*douly linked list end*/

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
int doMarkErrorsCommand(CMD* command,Board* board){
    int x = -1;

    /* convert params to int */
    if (!convertCommandToInt(command,1)){
        return 0;
    }
    x = (&(command->args))->x_int;

    /* check params validity */
    if (x > 1 || x <0 ){
        printf(PARAMETER_X_ILLEGAL_ERROR);
        return 0;
    }
    /* the params are valid !*/
    setMarkErrors(board, x);
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
int doUndoCommand(DubList* list){
	if (goStepBack(list) )
	{
        return 1;
	}
	return 0;
}
int doRedoCommand(DubList* list){
    if (goStepForward(list) )
    {
        return 1;
    }
    return 0;
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



int doSetCommand(CMD* command,Board* board){

    int x = -1;
    int y = -1;
    int z = -1;

    /* convert params to int */
    if (!convertCommandToInt(command,3)){
        return 0;
    }
    x = (&(command->args))->x_int;
    y = (&(command->args))->y_int;
    z = (&(command->args))->z_int;

    printf("x = %i ,y = %i ,z = %i \n",x,y,z);

	/* the params are numerical , check validity */
    /* Adi likes y,x instead of x,y */
    checkRowColValid(board,y,x);
    if (z > board->N || z <0 ){
        printf("Error: Parameter z value is illegal\n");
        return 0;
    }
    /* the params are valid !*/

    if ( !setVal(board, y-1, x-1, z) ){
        printf("Error: board not valid");
        return 0;
    }
    return 1;
}

bool do_commands(CMD* command, Board* board,DubList* moves){

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
	    if (doMarkErrorsCommand(command,board))
	    {
	    	printBoard(board);
	    }
	    break;
	case PRINT_BOARD:
	    printBoard(board);
	    break;

	case SET:
	    printf("set cmd\n");
	    if (doSetCommand(command,board))
	    {
            return true;
	    }
        return false;

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
        if (doUndoCommand(moves))
        {
            return true;
        }
        return false;
	case REDO:
	    printf("redo cmd\n");
	    if (doRedoCommand(moves))
	    {
            return true;
	    }
        return false;
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

	int is_over = 0;
    DubList dlist = {0};
	DubList* moves = &dlist;


	Board* old_board;
    Board* new_board;
    old_board = createEmptyBoard(3,3);
    pushToList(moves,old_board);
	printf("play game\n");
	printBoard(old_board);
	

	while(1){
		CMD cmd = {0};
		char cmd_text[CMD_MAX_LENGTH + 1] = {0};
		old_board = (moves->curr)->board;
        new_board = createEmptyBoard(3,3);

        copyBoard(new_board,old_board);

		if (!get_command(cmd_text))
		{
			break;
		}
		if ( !parse_command(cmd_text,&cmd) )
		{
			continue;
		} 
		if (!do_commands(&cmd, new_board,moves)) {
			continue;
		}
		/* Conmnand successded !*/
		if (cmd.type != UNDO && cmd.type != REDO)
		{
            pushToList(moves,new_board);
		}

		/*
		 * TODO : if exiting remember to fre new_board
		printf("oldd board %p \n",old_board);
		printf("new board %p \n",new_board);
		printList(moves);
		*/
		printBoard((moves->curr)->board);
		
	}

	return is_over;

}
