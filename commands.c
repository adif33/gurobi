#include "commands.h"



int doSolveCommand(CMD* command, Board** board_ptr){
    freeBoard(*board_ptr);

    *board_ptr = loadBoard(command->x);
    (*board_ptr)->curr_mode = solve;

    return 1;
}

int doEditCommand(CMD* command, Board** board_ptr){
    /*TODO: handle errors in loadBoard*/
    printf("param x : %s \n", command->x);
    freeBoard(*board_ptr);
    if(!command->x){
        *board_ptr = createInitBoard();
    } else{
        *board_ptr = loadBoard(command->x);
    }


    return 1;
}
int doMarkErrorsCommand(CMD* command,Board* board){
    int x = -1;

    /* convert params to int */
    if (!convertCommandToInt(command,1)){
        return 0;
    }
    x = command->x_int;

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
    printf("param x : %s \n", command->x);
    return 1;
}
int doGuessCommand(CMD* command){
    printf("param x : %s \n", command->x);
    printf("test\n");
    return 1;
}
int doGenerateCommand(CMD* command){
    printf("param x : %s \n", command->x);
    printf("param y : %s \n", command->y);
    return 1;
}
bool doUndoCommand(DubList* list,Board** board_ptr){
    Board* board = *board_ptr;
    if (goStepBack(list) )
    {
        printf("freeing board %p \n",(void*) board);
        freeBoard(board);
        *board_ptr = creatCopiedBoard((list->curr)->board);
        return true;
    }
    return false;
}
bool doRedoCommand(DubList* list,Board** board_ptr){
    Board* board = *board_ptr;
    if (goStepForward(list) )
    {
        printf("freeing board %p \n",(void*) board);
        freeBoard(board);
        *board_ptr = creatCopiedBoard((list->curr)->board);
        return true;
    }
    return false;
}
bool doSaveCommand(CMD* command, Board** board_ptr){
    /*TODO: handle errors in saveBoard*/
    printf("param x : %s \n", command->x);
    if((*board_ptr)->curr_mode != edit && (*board_ptr)->curr_mode != solve){
        printf("ERROR: wrong mode");
        return false;
    }

    if(!command->x){
        printf("ERROR: no x value");
        return false;
    }

    if ((*board_ptr)->curr_mode == edit){
        if (isErroneous(*board_ptr)){
            printf("ERROR: Board is Erroneous in edit mode");
            return false;
        }
        if (getNumberOfSolution(*board_ptr)<1){
            printf("ERROR: No possible solutions in edit mode");
            return false;
        }
    }
    saveBoard(*board_ptr, command->x);

    return true;
}
int doHintCommand(CMD* command){
    printf("param x : %s \n", command->x);
    printf("param y : %s \n", command->y);
    return 1;
}
int doGuessHintCommand(CMD* command){
    printf("param x : %s \n", command->x);
    printf("param y : %s \n", command->y);
    return 1;
}
int doNumSolutionsCommand(CMD* command){
    printf("param x : %s \n", command->x);
    return 1;
}
int doAutofillCommand(CMD* command){
    printf("param x : %s \n", command->x);
    return 1;
}
int doResetCommand(CMD* command){
    printf("param x : %s \n", command->x);
    return 1;
}
bool doExitCommand(CMD* command,DubList* moves){
    moves->isOver = true;
    return 1;
}



bool doSetCommand(CMD* command,Board* board){

    int x = -1;
    int y = -1;
    int z = -1;

    /* convert params to int */
    if (!convertCommandToInt(command,3)){
        return 0;
    }
    x = command->x_int;
    y = command->y_int;
    z = command->z_int;

    printf("x = %i ,y = %i ,z = %i \n",x,y,z);

    /* the params are numerical , check validity */
    /* Adi likes y,x instead of x,y */
    if (!checkRowColValid(board,y,x) ){
        return 0;
    }
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

bool do_commands(CMD* command, Board** board_ptr,DubList* moves){
    Board* board;
    /*
     * WATCH THIS NEW LINE |
     *                    \/
     */
    board = *board_ptr;

    switch (command->type) {
        case SOLVE:
            doSolveCommand(command, board_ptr);
            break;

        case EDIT:
            doEditCommand(command, board_ptr);
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
            if (doSetCommand(command, board))
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
            if (doUndoCommand(moves,board_ptr))
            {
                return true;
            }
            return false;
        case REDO:
            printf("redo cmd\n");
            if (doRedoCommand(moves,board_ptr))
            {
                return true;
            }
            return false;

        case SAVE:
            printf("save cmd\n");
            if (doSaveCommand(command, board_ptr))
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
            doExitCommand(command,moves);
            return false;


    }

    return true;

}

