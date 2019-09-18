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
        if (cmd.type == RESET)
        {
            printf("Bye bye");
            break;
        }

        if (!do_commands(&cmd, &board,moves))
        {
            continue;
        }
        if (cmd.type != UNDO && cmd.type != REDO)
        {
            pushToList(moves, creatCopiedBoard(board));
        }
        if (isAllCellsFull(board) && (board->curr_mode==solve)){
            if( isBoardSolved(board)){
                printf(SOLVED_MESSAGE);
                board->curr_mode = init;
            } else{
                printf(NOT_SOLVED_MESSAGE);
            }
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


