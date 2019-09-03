
#ifndef SOLVER_H_   /* Include guard */
#define SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game.h"

#define MAX_STACK_CAPACITY (10000)

typedef struct stackItem{
    Board* board;
    int row;
    int column;

} StackItem;

typedef struct stack
{
    int maxsize;
    int top;
    StackItem* *items;
} Stack;

/* solve using exhaustive backtracking and return number of possibilities*/
int getNumberOfSolution(Board* board);

/* fill random x cells and solve using ILP. than remove random Y values*/
bool generate(Board* board, int x, int y);

#endif /* SOLVER_H_ */