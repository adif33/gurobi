
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


bool isBoardSolved(Board* board){
    int N, row, column;
    Cell* curr_cell;

    setErroneous(board);



    N = board->N;

    for(row=0; row<N; row++){
        for(column=0; column<N; column++){
            curr_cell = getCell(board, row, column);
            if (curr_cell->erroneous || (curr_cell->value==0)){
                return false;
            }

        }
    }
    return true;

}

bool isErroneous(Board* board){
    int N, row, column;
    Cell* curr_cell;

    setErroneous(board);



    N = board->N;

    for(row=0; row<N; row++){
        for(column=0; column<N; column++){
            curr_cell = getCell(board, row, column);
            if (curr_cell->erroneous){
                return false;
            }

        }
    }
    return true;

}






