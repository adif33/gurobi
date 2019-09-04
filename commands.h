#ifndef COMMANDS_H
#define COMMANDS_H

#include "game.h"
#include "loader.h"
#include "main_aux.h"


typedef struct node Node;
struct node {
    Board* board;
    Node* next;
    Node* prev;
} ;

typedef struct dublist {
    Node* curr;
} DubList;

typedef enum cmdtype {
    SOLVE,
    EDIT,
    MARK_ERRORS,
    PRINT_BOARD,
    SET,
    VALIDATE,
    GUESS,
    GENERATE,
    UNDO,
    REDO,
    SAVE,
    HINT,
    GUESS_HINT,
    NUM_SOLUTIONS,
    AUTOFILL,
    RESET,
    EXIT,
    SKIP} CMDType;


typedef struct {
    CMDType type;
    char* x;
    char* y;
    char* z;
    int x_int;
    int y_int;
    int z_int;
} CMD;


bool do_commands(CMD* command, Board** board_ptr);

#endif
