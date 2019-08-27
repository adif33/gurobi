
#include "logic.h"


bool gotHorizontalDuplicate(Board* board, int row, int column){
    int val, i;

    val = board->cells[row][column].value;
    for(i=0; i<board->N; i++){
        if (i == column) {
            continue;
        } else if (board->cells[row][i].value == val){
            return true;
        }

    }

    return false;
}

bool gotVerticalDuplicate(Board* board, int row, int column){
    int val, i;

    val = board->cells[row][column].value;
    for(i=0; i<board->N; i++){
        if (i == row) {
            continue;
        } else if (board->cells[i][column].value == val){
            return true;
        }

    }

    return false;
}

bool gotBlockDuplicate(Board* board, int row, int column){
    int i, j, val, block_start_row, block_end_row, block_start_column, block_end_column;
    val = board->cells[row][column].value;

    block_start_row = (row/board->m)*board->m;
    block_start_column= (column/board->n)*board->n;

    block_end_row = (row/board->m+1)*(board->m);
    block_end_column= (column/board->n+1)*(board->n);

    for(i=block_start_row; i<block_end_row; i++){
        for(j=block_start_column; j<block_end_column; j++){
            if (i==row && j==column){
                continue;
            } else if(board->cells[i][j].value == val){
                return true;
            }
        }

    }

    return false;
}


void setErroneous(Board* board){
    int row, column, val;
    for(row=0; row<board->N; row++){
        for(column=0; column < board->N; column++){
            val = board->cells[row][column].value;
            if ((gotHorizontalDuplicate(board, row, column) ||
            gotVerticalDuplicate(board, row, column) ||
            gotBlockDuplicate(board, row, column)) && (val != 0)){
                board->cells[row][column].erroneous = true;
            }
        }
    }
}

