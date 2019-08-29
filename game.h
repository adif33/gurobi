#ifndef GAME_H_   /* Include guard */
#define GAME_H_
#define DEFAULT_BLOCK_SIZE (3)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef enum mode{
    init,
    solve,
    edit} Mode;

typedef struct cell
{
    int value;
    int correct_value; /* can be used to store temporary value for solution phase */
    
    bool fixed;
    bool erroneous;
    
    
} Cell;

typedef struct board
{
    int n; /*n value*/
    int m; /*m value*/
    int N; /* n*m */
    
    Cell **cells;
    
    Mode curr_mode;
    bool mark_errors;

} Board;



Board* createEmptyBoard(int n, int m);
Board* createInitBoard();

bool setVal(Board* board, int row, int col, int value);
void setFixed(Board* board, int row, int col, bool fixed);

/*
 * check if there is colision of value with row and column
 */
bool isInRowAndColumn(Board* board, int val, int col, int row);

/*
 * check if there is colision of value with the block
 */
bool isInBlock(Board* board, int val, int col, int row);

int getVal(Board* board, int col, int row);

/*
 * set value if legal and return true if success
 */


/*
 * This function checks whether the puzzle is solved and the game is over.
 */
bool isSolved(Board* board);

void printBoard(Board* board);

int getCellValue(Board* board, int row, int column);

bool isCellEmpty(Board* board, int row, int column);

Cell* getCell(Board* board, int row, int column);


bool gotHorizontalDuplicate(Board* board, int row, int column);

bool gotVerticalDuplicate(Board* board, int row, int column);

bool gotBlockDuplicate(Board* board, int row, int column);

void setMarkErrors(Board* board, bool value);

#endif /* GAME_H_ */
