#ifndef ILP_H_   /* Include guard */
#define ILP_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "game.h"
#include "logic.h"
#include "config.h"


typedef enum status{
    validate,
    generate,
    guess,
    guess_hint} Status;

typedef struct solution {
    int error;
    bool solved ;
    Status stat;
    double thres;
    int row;
    int col;
} Solution;


bool validateBoard(Board* board);

bool generateBoard(Board** board_ptr,int numFill, int numRemove );

bool guessBoard(Board* board, double threshold);

bool guessHintBoard(Board* board, int row,int col);

bool hint(Board* board, int row,int col);




#endif /* ILP_H_ */
