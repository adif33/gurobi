#include <string.h>

#include "game.h"
#include "main_aux.h"


void freeBoard(Board* board){
    int row;
    Cell* curr_cell;

    /*printf("freeing board %p \n",(void*)board);*/
    for(row=0; row<board->N; row++){
        curr_cell = getCell(board, row, 0);
        free(curr_cell);
    }
    free(board->cells);
    free(board);

}

Board* createEmptyBoard(int n, int m){
    /* return an empty board with n and m*/
    Board *board = (Board*)malloc(sizeof(Board));
    Cell** cells;
    int i, j;
    
    board->n = n;
    board->m = m;
    board->N = board->n*board->m;
    board->curr_mode = edit;
    
    cells = malloc(board->N*sizeof(Cell*));
    for (i=0; i<board->N; i++)
        cells[i] = (Cell*)malloc(board->N*sizeof(Cell));
    
    for (i=0; i<board->N; i++){
        for (j=0; j<board->N; j++){
            cells[i][j].value = 0;
            cells[i][j].correct_value = 0;
            cells[i][j].fixed = false;
            cells[i][j].erroneous = false;
        }
    }
    
    board->cells = cells;
    
    return board;
    
}

Board* createInitBoard(){
    return createEmptyBoard(DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
}

Cell* getCell(Board* board, int row, int column){
    return &board->cells[row][column];
}

int getCellValue(Board* board, int row, int column){
    Cell* curr_cell;
    curr_cell = getCell(board, row, column);
    return curr_cell->value;
}

bool isCellEmpty(Board* board, int row, int column){
    Cell* curr_cell;
    curr_cell = getCell(board, row, column);
    return (curr_cell->value == 0);
}

bool isCellFixed(Board* board, int row, int column){
    Cell* curr_cell;
    curr_cell = getCell(board, row, column);
    return curr_cell->fixed;
}

void printHorizontalSeperator(int N, int m){
    int i;
    for(i=0; i<(4*N+m+1); i++) {
        printf("-");
    }
    printf("\n");
}


void printVerticalSeperator(){
    printf("|");
}

void printPoint(Board* board, int row, int column){
    /*return the aproproate point if errno or foxed or nothing*/
    Cell curr_cell;
    curr_cell = board->cells[row][column];

    if (curr_cell.fixed) {
        printf(".");
    } else if (curr_cell.erroneous && (board->mark_errors || board->curr_mode == edit)){
        printf("*");
    } else{
        printf(" ");
    }



}


bool setVal(Board* board, int row, int col, int value){
    /* set value of a cell*/
    Cell* curr_cell;

    if(board->curr_mode == init){
        return false;
    }
    curr_cell = getCell(board, row, col);
    if (curr_cell->fixed){
        return false;
    }
    curr_cell->value = value;
    return true;

}

void printBoard(Board* board){
    /*will print the board*/
    int n, m, row, column, N;

    n = board->n;
    m = board->m;
    N = board->N;

    setErroneous(board);
    for(row=0; row<N; row++){
        if (row%m == 0){
            printHorizontalSeperator(N, m);
        }
        for(column=0; column<N; column++){
            if (column%n == 0){
                printVerticalSeperator();
            }

            if (isCellEmpty(board, row, column)){
                printf("   ");
            } else{
                printf(" %2d", board->cells[row][column].value);
            }
            printPoint(board, row, column);
            /*val = cells[row][column].value ? cells[row][column].value;*/


        }
        printf("|\n");
    }
    printHorizontalSeperator(N, m);

}

bool gotHorizontalDuplicate(Board* board, int row, int column){
    /*return if a cell got dulpicate*/
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
    /*return if a cell got dulpicate*/
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
    /*return if a cell got dulpicate*/

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


void setMarkErrors(Board* board, bool value){
    board->mark_errors = value;
}

void setFixed(Board* board, int row, int col, bool fixed){
    /* set cell as fixed */
    Cell* curr_cell;

    curr_cell = getCell(board, row, col);
    curr_cell->fixed = fixed;
}


bool copyBoard(Board* dst, Board* src){
    /* copy board from one pointer to second */
    int N;
    int i =0 ;
    int j = 0;
    Cell* src_cell;
    Cell* dst_cell;
    if (src->N != dst->N ){
        printf("Error: boards not same size ! \n");
        return 0;
    }

    N = src->N;

    dst->curr_mode = src->curr_mode;

    for (i = 0; i < N ; ++i) {
        for (j = 0; j < N ; ++j) {
            /*
            printf("i,j: %i,%i value: %i \n",i,j,getCellValue(src , i, j));
            */
            src_cell = getCell(src,i,j);
            dst_cell = getCell(dst,i,j);
            memcpy(dst_cell,src_cell, sizeof(Cell));
        }

    }
    return 1;
}

bool checkRowColValid(Board* board,int row, int column){
    if (column > board->N -1 || column < 0 ){
        printf(PARAMETER_X_NOT_IN_RANGE_ERROR);
        /*printf("Error: value not in range %i-%i\n",0,board->N);*/
        return false;
    }
    if (row > board->N - 1 || row < 0 ){
        printf(PARAMETER_Y_NOT_IN_RANGE_ERROR);
        return false;
    }
    return true;
}

Board* creatCopiedBoard(Board* old_board){
    /* create a copy and return pointer*/
    Board* new_board;
    int row, column, N;
    Cell* src_cell;
    Cell* dst_cell;

    new_board = createEmptyBoard(old_board->n, old_board->m);
    N = new_board->N;

    for(row=0; row<N; row++) {
        for (column = 0; column < N; column++) {
            src_cell = getCell(old_board, row, column);
            dst_cell = getCell(new_board, row, column);
            memcpy(dst_cell, src_cell, sizeof(Cell));
        }
    }

    return new_board;
}


void emptyCorrectValues(Board* board){
    int row, column, N;
    Cell* curr_cell;

    N = board->N;


    for(row=0; row<N; row++) {
        for (column = 0; column < N; column++) {
            curr_cell = getCell(board, row, column);
            curr_cell->correct_value = 0;
        }
    }
    return;

}


int getCorrectValue(Board* board, int row, int column){
    Cell* curr_cell;
    curr_cell = getCell(board, row, column);
    return curr_cell->correct_value;
}


void delta(Board* old_board, Board* new_board){
    int row, column, N;

    int new_value, old_value;

    N = old_board->N;

    for(row=0; row<N; row++) {
        for (column = 0; column < N; column++) {
            new_value = getCell(new_board, row, column)->value;
            old_value = getCell(old_board, row, column)->value;
            if (old_value != new_value){
                printf(DELTA_PRINT, column+1, row+1, old_value, new_value);
            }
        }
    }
}
