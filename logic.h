
#ifndef LOGIC_H_   /* Include guard */
#define LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game.h"




void setErroneous(Board* board);

bool isErroneous(Board* board);

bool isBoardSolved(Board* board);

int detectLegalValues(Board* board, int row, int col, bool* values);


#endif /* LOGIC_H_ */
