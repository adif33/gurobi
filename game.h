#ifndef GAME_H_   /* Include guard */
#define GAME_H_

enum mode{init, solve, edit}

typedef struct board
{
    int n; /*n value*/
    int m; /*m value*/
    int N; /*not sure yet*/
    
    cell **cells;
    
    enum mode curr_mode
    bool mark_errors

} Board;

typedef struct cell
{
    int value;
    int correct_value; /* can be used to store temporery value for solution phase */
    
    bool fixed;
    bool erroneous;
    
    
} Cell;

Board* createBoard(int col, row);

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
 * set value
 */
void setVal(Board* board, int col, int row, int value);

/*
 * set value if legal and return True is success
 */
bool trySetVal(Board* board, int col, int row, int value);

/*
 * This function checks whether the puzzle is solved and the game is over.
 */
bool isSolved(Board* board);

#endif /* GAME_H_ */
