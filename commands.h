#ifndef COMMANDS_H
#define COMMANDS_H

struct CMD;

#include "game.h"
#include "loader.h"
#include "logic.h"
#include "solver.h"
#include "ilp_solver.h"
#include "config.h"


typedef struct node Node;
struct node {
    Board* board;
    Node* next;
    Node* prev;
} ;

typedef struct dublist {
    Node* curr;
    bool isOver;
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


bool do_commands(CMD* command, Board** board_ptr,DubList* list);
int pushToList(DubList* list, Board* boardPtr);
bool freeList(DubList* list);

#endif
