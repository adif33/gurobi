
#include "logic.h"





void setErroneous(Board* board){
    int row, column, val;
    for(row=0; row<board->N; row++){
        for(column=0; column < board->N; column++){
            val = board->cells[row][column].value;
            if ((gotHorizontalDuplicate(board, row, column) ||
            gotVerticalDuplicate(board, row, column) ||
            gotBlockDuplicate(board, row, column)) && (val != 0)){
                board->cells[row][column].erroneous = true;
            } else{
                board->cells[row][column].erroneous = false;
            }
        }
    }
}

