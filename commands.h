#ifndef COMMANDS_H
#define COMMANDS_H

#include "game.h"
#include "loader.h"
#include "parser.h"
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

bool do_commands(CMD* command, Board** board_ptr);

#endif
