#include <stdio.h>
#include <stdlib.h>

#include "main_aux.h"


void clearCMD(CMD* cmd){
    cmd->x = '\0';
    cmd->y = '\0';
    cmd->z = '\0';
    cmd->x_int = -1;
    cmd->y_int = -1;
    cmd->z_int = -1;
}

bool mainLoop(){
    /* this is the main part of the code*/
    DubList dlist = {0};
    DubList* moves = &dlist;
    Board* board = createInitBoard(); /*creating empty board*/
    CMD cmd = {0};
    char cmd_text[CMD_MAX_LENGTH + 1] = {0};

    board->curr_mode = init; /*setting mode to init*/


    while (true){
        clearCMD(&cmd);

        if (moves->isOver || !get_command(cmd_text) )
        {
            break;
        }

        if (!parse_command(cmd_text, &cmd, board))  /*here we get the data from each commands*/
        {
            continue;
        }
        if (!do_commands(&cmd, &board,moves)) /* this will preform the command*/
        {
            continue;
        }
        setErroneous(board);
        if (cmd.type != UNDO && cmd.type != REDO && cmd.type != RESET)
        {
            pushToList(moves, creatCopiedBoard(board));
        }

        if (isAllCellsFull(board) && (board->curr_mode==solve)){ /*check if we finished the game */
            if( isBoardSolved(board)){
                printf(SOLVED_MESSAGE);
                board->curr_mode = init;
            } else{
                printf(NOT_SOLVED_MESSAGE);
            }
        }

        if((cmd.type!=SOLVE) && (cmd.type!=EDIT) && (cmd.type!=NUM_SOLUTIONS) && (cmd.type!=EDIT)){
            printBoard(board);
        }

    }

    freeBoard(board);
    if (freeList(moves)){
        return true;
    }
    return false;

}


