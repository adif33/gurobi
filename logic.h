
#ifndef LOGIC_H_   /* Include guard */
#define LOGIC_H_

include "game.h"

/* return if the board is valid (no prints)*/
bool validate(Board* board);

/* solve using LP. not sure what it is (no prints)*/
bool guess(Board* board, float x);

/* fill random x cells and solve using ILP. than remove random Y values*/
bool generate(Board* board, int x, int y);

/* solve using ILP and set values in the "correct_value" of each cell*/
bool solveILP(Board* board);

/* solve using LP and set values in the "correct_value" of each cell*/
bool solveLP(Board* board);


/* solve using exhaustive backtracking and return number of possibilities*/
int getNumberOfSolution(Board* board);

bool autofill(Board* board);

/* return the legal posibilities for a cell */
int* getPossiblePositioning(Board* board, int row, int column)

/* return the legal posibilities for a cell */
int* getPossiblePositioningBlock(Board* board, int row, int column);

int* getPossiblePositioningRow(Board* board, int row, int column);

int* getPossiblePositioningColumn(Board* board, int row, int column);




#endif /* LOGIC_H_ */
