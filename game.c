#include "game.h"


Board* createEmptyBoard(){
    Board *board = (Board*)malloc(sizeof(Board));
    Cell** cells;
    int i, j;
    
    board->n = DEFAULT_BLOCK_SIZE;
    board->m = DEFAULT_BLOCK_SIZE;
    board->N = board->n*board->m;
    board->curr_mode = edit;
    
    cells = malloc(board->N*sizeof(Cell*));
    for (i=0; i<board->N; i++)
        cells[i] = (Cell*)malloc(board->N*sizeof(Cell));
    
    for (i=0; i<board->N; i++){
        for (j=0; j<board->N; j++){
            /*cells[i][j] = (Cell)malloc(sizeof(Cell));*/
            cells[i][j].value = 0;
            cells[i][j].correct_value = 0;
            cells[i][j].fixed = false;
            cells[i][j].erroneous = false;
        }
    }
    
    board->cells = cells;
    
    return board;
    
}

bool isCellEmpty(Board* board, int row, int column){
    return board->cells[row][column].value == 0;
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

void printBoard(Board* board){
    int n, m, row, column, N, val;

    n = board->n;
    m = board->m;
    N = board->N;

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











