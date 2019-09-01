#include <stdio.h>

#include "game.h"
#include "main_aux.h"
#include "logic.h"
#include "loader.h"


int main(int argc, char *argv[]) {
    int status = 0;
    bool b;
    Board* board;


	printf("Welcome to the Gurobi game ! \n");

	/*status = play_game();*/
/*
    board = createInitBoard();
    board->cells[1][1].value=5;
    board->cells[0][0].value=5;
    board->cells[1][2].value=4;
    board->cells[1][5].value=4;
    board->cells[1][0].value=6;
    setErroneous(board);
    b = setVal(board, 1, 5, 1);
    board->cells[1][5].fixed=true;
    setErroneous(board);
    printBoard(board);
    saveBoard(board, "asd");
    */
    loadBoard("./gurobi_save2");


    return status;
}
