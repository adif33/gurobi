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
    
    board->cells = cells;
    
    
    
}


void printBoard(){
	printf("print board\n");
}