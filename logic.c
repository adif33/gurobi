
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
                return true;
            }
        }
    }
    return false;
}

/* Warning !
 * this function assumes that the given cell is empty
 * Recieves a bool array of size N+1
 * ignores the values[0], and set for each value[i]
 * T/F if the value i is legal / not
 * returns the number of legal options
 */
int detectLegalValues(Board* board, int row, int col, bool* values){
    int i,j,tmp,count,N,m,n;
    N = board->N;
    count = N;
    n = board->n;
    m = board->m;

    /*reset to true*/
    for ( i = 1; i < N +1 ; ++i) {
        values[i] = true;
    }

    /*row and col */
    for ( i = 0; i < N ; ++i) {
        tmp = getCellValue(board,row,i);
        if (tmp != 0){
            if (values[tmp] ==1 ){
                values[tmp] = 0;
                count -=1;
            }
        }
        tmp = getCellValue(board,i,col);
        if (tmp != 0){
            if (values[tmp] ==1 ){
                values[tmp] = 0;
                count -=1;
            }
        }
    }

    /*block */
    for ( i = (row / m)*m ; i < ((row / m)*m + m); ++i) {
        for ( j = (col /n)*n ; j < ((col /n)*n + n ); ++j) {
            tmp = getCellValue(board,i,j);
            if (tmp != 0){
                if (values[tmp] ==1 ){
                    values[tmp] = 0;
                    count -=1;
                }
            }
        }
    }

    return count;
}






