
#ifndef LOGIC_H_   /* Include guard */
#define LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game.h"




void setErroneous(Board* board);
bool isErroneous(Board* board);

bool isBoardSolved(Board* board);


#endif /* LOGIC_H_ */
