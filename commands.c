#include "commands.h"

bool convertCommandToInt(CMD* command,int argsNum,Board* board){
    int x = -1;
    int y = -1;
    int z = -1;
    char *endptr;

    x = strtol(command->x,&endptr, 10);
    if (endptr == command->x )
    {
        printf(PARAMETER_X_CONVERSION_ERROR);
        return false;
    }
    command->x_int = x;
    if (argsNum == 1){
        return true;
    }

    y = strtol(command->y,&endptr, 10);
    if (endptr == command->y )
    {
        printf(PARAMETER_Y_CONVERSION_ERROR);
        return false;
    }
    command->y_int = y;
    if (argsNum == 2){
        return true;
    }

    z = strtol(command->z,&endptr, 10);
    if (endptr == command->z )
    {
        printf(PARAMETER_Z_CONVERSION_ERROR);
        return false;
    }
    command->z_int = z;
    return true;
}

/*DOUBLY LINKED LIST - UNDO REDO*/

void freeNextNodes(Node * node){
    Node * tmp ;
    Node * tmp1 ;
    tmp = node;
    while (tmp != NULL ){
        tmp1 = tmp->next;
        /*printf("freeing node %p , board %p \n",(void*)tmp,(void*)tmp->board);*/
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


bool goStepBack(DubList* list){
    Node * node ;
    node = list->curr;

    if (node == NULL ){
        printf("Error: list is empty\n");
        return false;
    } else if (node->prev == NULL){
        printf("Error: no more undos \n");
        return false;
    } else {
        list->curr = node->prev;
        return true;
    }
}
bool goStepForward(DubList* list){
    Node * node ;
    node = list->curr;

    /*printf("redo \n");
    printList(list);*/
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

bool freeList(DubList* list){
    Node* node = list->curr;
    Node* tmp;

    if (node == NULL){
        return true;
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
    return true;
}

bool doReset(DubList* list){
    Node* node = list->curr;
    Node* tmp;

    if (node == NULL){
        return true;
    }

    /*Go back all the way*/
    tmp = node->prev;

    while (tmp != NULL )
    {
        node = tmp;
        tmp = node->prev;

    }
    /*free all next nodes*/
    list->curr = node;
    /*printList(list);*/
    return true;
}

/*douly linked list end*/



int doSolveCommand(CMD* command, Board** board_ptr, DubList* moves){
    Board* tmp_board;

    tmp_board = loadBoard(command->x);
    if ((tmp_board==NULL)){
        return 0;
    }

    freeBoard(*board_ptr);
    *board_ptr = tmp_board;
    (*board_ptr)->curr_mode = solve;

    freeList(moves);

    return 1;
}

int doEditCommand(CMD* command, Board** board_ptr, DubList* moves){
    /*TODO: handle errors in loadBoard
    printf("param x : %s \n", command->x);*/
    Board* tmp_board;
    int N, row, column;
    Cell* curr_cell;


    if(!command->x){
        tmp_board = createInitBoard();
    } else{
        tmp_board = loadBoard(command->x);
        if ((tmp_board==NULL)){
            return 0;
        }
    }
    N = tmp_board->N;

    for(row=0; row<N; row++){
        for(column=0; column<N; column++){
            curr_cell = getCell(tmp_board, row, column);
            curr_cell->fixed = false;


        }
    }


    freeBoard(*board_ptr);
    *board_ptr = tmp_board;

    freeList(moves);


    return 1;
}
int doMarkErrorsCommand(CMD* command,Board* board){
    int x = -1;

    /* convert params to int */
    if (!convertCommandToInt(command,1,board)){
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
    return 0;
}

bool doValidateCommand(Board* board){
    if (isErroneous(board)){
        printf(ERRONEOUS_BOARD_ERROR);
        return false;
    }
    validateBoard(board);
    return false;
}

bool doGuessCommand(CMD* command,Board* board){
    double thres ;
    int error ;

    error = sscanf(command->x,"%lf",&thres);
    if (error == 0){
        printf(PARAMETER_X_NOT_DOUBLE_ERROR);
        return false;
    }

    if (isErroneous(board)){
        printf(ERRONEOUS_BOARD_ERROR);
        return false;
    }
    return guessBoard(board,thres);
}
bool doGenerateCommand(CMD* command,Board** board_ptr){
    Board* board;
    board = *board_ptr;

    if (!convertCommandToInt(command,2,board)){
        return false;
    }
    if (command->x_int < 0 || command->x_int > board->N*board->N ){
        printf(GENERATE_PARAMETER_X_NOT_IN_RANGE_ERROR);
        return false;
    }
    if (command->y_int < 1 || command->y_int > board->N*board->N ){
        printf(GENERATE_PARAMETER_Y_NOT_IN_RANGE_ERROR);
        return false;
    }

    if (isErroneous(board)){
        printf(ERRONEOUS_BOARD_ERROR);
        return false;
    }

    return generateBoard(board_ptr,command->x_int,command->y_int);
}
bool doUndoCommand(DubList* list,Board** board_ptr){
    Board* board = *board_ptr;
    if (goStepBack(list) )
    {
        *board_ptr = creatCopiedBoard((list->curr)->board);
        delta(board, *board_ptr);
        freeBoard(board);
        return true;
    }
    return false;
}
bool doRedoCommand(DubList* list,Board** board_ptr){
    Board* board = *board_ptr;
    if (goStepForward(list) )
    {
        *board_ptr = creatCopiedBoard((list->curr)->board);
        delta(board, *board_ptr);
        freeBoard(board);
        return true;
    }
    return false;
}
bool doSaveCommand(CMD* command, Board** board_ptr){
    /*TODO: handle errors in saveBoard*/
    int row, column, N;
    Cell* curr_cell;
    Board* tmp_board;

    tmp_board = *board_ptr;

    if((*board_ptr)->curr_mode != edit && (*board_ptr)->curr_mode != solve){
        printf(WRONG_MODE_ERROR);
        return false;
    }

    if(!command->x){
        printf("ERROR: no x value\n");
        return false;
    }

    if ((*board_ptr)->curr_mode == edit){
        if (isErroneous(*board_ptr)){
            printf("ERROR: Board is Erroneous in edit mode\n");
            return false;
        }
        if (getNumberOfSolution(*board_ptr)<1){
            printf("ERROR: No possible solutions in edit mode\n");
            return false;
        }
    }
    N = tmp_board->N;
    if ((*board_ptr)->curr_mode == edit){
        for(row=0; row<N; row++){
            for(column=0; column<N; column++){
                curr_cell = getCell(tmp_board, row, column);
                if(curr_cell->value!=0){
                    curr_cell->fixed = true;
                }

            }
        }
    }

    saveBoard(*board_ptr, command->x);

    if ((*board_ptr)->curr_mode == edit){
        for(row=0; row<N; row++){
            for(column=0; column<N; column++){
                curr_cell = getCell(tmp_board, row, column);
                curr_cell->fixed = false;

            }
        }
    }

    /*return return_value;*/
    return true;
}
bool doHintCommand(CMD* command,Board* board){
    int x,y;
    /* convert params to int */
    if (!convertCommandToInt(command,2,board)){
        return false;
    }
    x = command->x_int-1;
    y = command->y_int-1;

    /* the params are numerical , check validity */
    /* Adi likes y,x instead of x,y */
    if (!checkRowColValid(board,y,x) ){
        return false;
    }

    if (isErroneous(board)){
        printf(ERRONEOUS_BOARD_ERROR);
        return false;
    }

    if (isCellFixed(board,y,x)){
        printf(FIXED_CELL_ERROR);
        return false;
    }
    if (getCellValue(board,y,x) !=  0){
        printf(NOT_EMPTY_ERROR);
        return false;
    }
    return hint(board,y,x);
}
bool doGuessHintCommand(CMD* command,Board* board){
    int x,y;
    /* convert params to int */
    if (!convertCommandToInt(command,2,board)){
        return false;
    }
    x = command->x_int-1;
    y = command->y_int-1;

    /* the params are numerical , check validity */
    /* Adi likes y,x instead of x,y */
    if (!checkRowColValid(board,y,x) ){
        return false;
    }

    if (isErroneous(board)){
        printf(ERRONEOUS_BOARD_ERROR);
        return false;
    }

    if (isCellFixed(board,y,x)){
        printf(FIXED_CELL_ERROR);
        return false;
    }
    if (getCellValue(board,y,x) !=  0){
        printf(NOT_EMPTY_ERROR);
        return false;
    }
    return guessHintBoard(board,y,x);
}
bool doNumSolutionsCommand(Board** board_ptr){
    int number;
    /*TODO: delete this check */
    if((*board_ptr)->curr_mode != edit && (*board_ptr)->curr_mode != solve){
        printf(WRONG_MODE_ERROR);
        return false;
    }

    if (isErroneous(*board_ptr)){
        printf(ERRONEOUS_BOARD_ERROR);
        return false;
    }

    number = getNumberOfSolution(*board_ptr);
    printf("Number of solutions: %d\n", number);

    return false; /*we didnt change the board so we dont want to update dublist*/
}

void doAutofillCommand(Board** board_ptr){
    int row, column, N, count, i, curr_value, curr_right_value;
    Cell* curr_cell;
    bool* bools;
    Board* board;

    board = *board_ptr;


    N = board->N;

    bools = (bool*)malloc(N*sizeof(bool));

    emptyCorrectValues(board);


    for(row=0; row<N; row++) {
        for (column = 0; column < N; column++) {
            /*for each cell*/
            curr_cell = getCell(board, row, column);

            if (curr_cell->value == 0){
                /*if empty cell*/
                count = detectLegalValues(board, row, column, bools);

                if (count==1){
                    /*only one possible value*/

                    for (i=1; i<=N; i++){

                        if(bools[i]){
                            /*set the value as correct_value*/
                            curr_cell->correct_value = i;

                        }
                    }
                }

            }
        }
    }

    free(bools);

    for(row=0; row<N; row++) {
        for (column = 0; column < N; column++) {
            curr_cell = getCell(board, row, column);
            curr_value = getCellValue(board, row, column);
            curr_right_value = getCorrectValue(board, row, column);
            if ((curr_value == 0) && (curr_right_value !=0)){
                setVal(board, row, column, curr_right_value);
                printf(CHANGE_CELL_VALUE_FORMAT, column+1, row+1, curr_right_value);
            }

        }
    }

}

bool doResetCommand(DubList* list,Board** board_ptr){
    doReset(list);

    freeBoard(*board_ptr);
    *board_ptr = creatCopiedBoard((list->curr)->board);
    return true;

}

bool doExitCommand(DubList* moves){
    moves->isOver = true;
    return 1;
}



bool doSetCommand(CMD* command,Board* board){

    int x = -1;
    int y = -1;
    int z = -1;

    /* convert params to int */
    if (!convertCommandToInt(command,3,board)){
        return 0;
    }
    x = command->x_int -1 ;
    y = command->y_int -1 ;
    z = command->z_int ;

    /*printf("x = %i ,y = %i ,z = %i \n",x,y,z);*/

    /* the params are numerical , check validity */
    /* Adi likes y,x instead of x,y */
    if (!checkRowColValid(board,y,x) ){
        return 0;
    }
    if (z > board->N || z <0 ){
        printf(PARAMETER_Z_NOT_IN_RANGE_ERROR);
        return 0;
    }
    /* the params are valid !*/

    if ( !setVal(board, y, x, z) ){
        printf("Error: board not valid\n");
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
            return doSolveCommand(command, board_ptr, moves);

        case EDIT:
            return doEditCommand(command, board_ptr, moves);

        case MARK_ERRORS:
            return doMarkErrorsCommand(command,board);

        case PRINT_BOARD:
            printBoard(board);
            return false;

        case SET:
            /*printf("set cmd\n");*/
            if (doSetCommand(command, board))
            {
                return true;
            }
            return false;

        case VALIDATE:
            return doValidateCommand(board);


        case GUESS:
            return doGuessCommand(command,board);

        case GENERATE:
            return doGenerateCommand(command,board_ptr);


        case UNDO:
            if (doUndoCommand(moves,board_ptr))
            {
                return true;
            }
            return false;
        case REDO:
            if (doRedoCommand(moves,board_ptr))
            {
                return true;
            }
            return false;

        case SAVE:
            return doSaveCommand(command, board_ptr);

        case HINT:
            return doHintCommand(command,board);

        case GUESS_HINT:
            return doGuessHintCommand(command,board);

        case NUM_SOLUTIONS:
            doNumSolutionsCommand(board_ptr);
            break;

        case AUTOFILL:
            doAutofillCommand(board_ptr);
            break;

        case RESET:
            return doResetCommand(moves,board_ptr);
        case EXIT:
            /*printf("Exit cmd\n");*/
            doExitCommand(moves);
            return false;
        case SKIP:
            return false;


    }

    return true;

}

