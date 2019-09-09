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

    x = strtol(command->x,&endptr, 10);
    if (endptr == command->x )
    {
        printf(PARAMETER_X_CONVERSION_ERROR);
        return 0;
    }
    command->x_int = x;
    if (argsNum == 1){
        return 1;
    }

    y = strtol(command->y,&endptr, 10);
    if (endptr == command->y )
    {
        printf(PARAMETER_Y_CONVERSION_ERROR);
        return 0;
    }
    command->y_int = y;
    if (argsNum == 2){
        return 1;
    }

    z = strtol(command->z,&endptr, 10);
    if (endptr == command->z )
    {
        printf(PARAMETER_Z_CONVERSION_ERROR);
        return 0;
    }
    command->z_int = z;
    return 1;
}
/*
 * assumes that all the
 */
int chooseRandomNumberByWeight(int* numbers,double* weights,int size){
    int i,random;
    int limit ;
    int factor = 100;
    double sum =0;

    for ( i = 0; i < size; ++i) {
        /*printf("number is: %i weight is:%f \n",numbers[i],weights[i]);*/
        sum += weights[i];
    }
    if (sum == 0){
        /*all weights are zero*/
        return 0;

    }
    limit = factor * sum;
    random = rand() % (limit ) + 1 ;

    /*printf("limit is: %i random is: %i \n",limit,random);*/

    for ( i = 0; i < size ; ++i) {
        if (random < weights[i]*factor ){
            return numbers[i];
        }
        random -= weights[i]*factor;
    }
    return 0;
}


void clearCMD(CMD* cmd){
    cmd->x = '\0';
    cmd->y = '\0';
    cmd->z = '\0';
    cmd->x_int = -1;
    cmd->y_int = -1;
    cmd->z_int = -1;
}

bool mainLoop(){
    DubList dlist = {0};
    DubList* moves = &dlist;
    Board* board = createInitBoard();
    CMD cmd = {0};
    char cmd_text[CMD_MAX_LENGTH + 1] = {0};

    /* TODO: remove this line and change createInitBoard function */
    board->curr_mode = init;


    while (true){
        clearCMD(&cmd);

        if (moves->isOver || !get_command(cmd_text) )
        {
            break;
        }

        if (!parse_command(cmd_text, &cmd, board))
        {
            continue;
        }

        if (!do_commands(&cmd, &board,moves))
        {
            continue;
        }
        if (cmd.type != UNDO && cmd.type != REDO)
        {
            pushToList(moves, creatCopiedBoard(board));
        }

        /*
        printList(moves);
        printf("board is: %p \n",(void*)board);
        */
        printBoard(board);
        }

    freeBoard(board);
    if (freeList(moves)){
        return true;
    }
    return false;

}


