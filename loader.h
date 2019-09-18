#ifndef LOADER_H
#define LOADER_H

#define DEFAULT_TEXT_LENGTH (1000)
#define DEFAULT_MATRIX_TEXT_SIZE (1000)

#include "game.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

bool saveBoard(Board* board, char* path);

char* createSavedBoardText(Board* board);

Board* loadBoard(char* path);



#endif
