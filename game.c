#include "game.h"



Board* createEmptyBoard(){
    Board *board = (Board*)malloc(sizeof(Board));
    Cell* cells[DEFAUL_BOARD_SIZE];
    int i, j;
    
    board->n = DEFAUL_BOARD_SIZE;
    board->m = DEFAUL_BOARD_SIZE;
    board->curr_mode = edit;
    
    
    for (i=0; i<DEFAUL_BOARD_SIZE; i++)
        cells[i] = (Cell*)malloc(DEFAUL_BOARD_SIZE*sizeof(Cell));
    
    for (i=0; i<DEFAUL_BOARD_SIZE; i++){
        for (j=0; j<DEFAUL_BOARD_SIZE; j++){
            /*cells[i][j] = (Cell)malloc(sizeof(Cell));*/
            cells[i][j].value = 0;
            cells[i][j].correct_value = -1;
            cells[i][j].fixed = false;
            cells[i][j].erroneous = false;
        }
    }
    
    board->cells = cells;
    
    return board;
    
}


void printBoard(){
	printf("print board\n");
}
