#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "game.h"
#include "main_aux.h"


int convertCommandToInt(CMD* command,int argsNum){
    int x = -1;
    int y = -1;
    int z = -1;
    char *endptr;

    x = strtol(command->x,&endptr, 10);
    if (endptr == command->x )
    {
        printf(PARAMETER_X_CONVERSION_ERROR);
        return 0;
    }
    command->x_int = x;
    if (argsNum == 1){
        return 1;
    }

    y = strtol(command->y,&endptr, 10);
    if (endptr == command->y )
    {
        printf(PARAMETER_Y_CONVERSION_ERROR);
        return 0;
    }
    command->y_int = y;
    if (argsNum == 2){
        return 1;
    }

    z = strtol(command->z,&endptr, 10);
    if (endptr == command->z )
    {
        printf(PARAMETER_Z_CONVERSION_ERROR);
        return 0;
    }
    command->z_int = z;
    return 1;
}

/*DOUBLY LINKED LIST - UNDO REDO*/

void freeNextNodes(Node * node){
    Node * tmp ;
    Node * tmp1 ;
    tmp = node;
    while (tmp != NULL ){
        tmp1 = tmp->next;
        printf("freeing node %p , board %p \n",(void*)tmp,(void*)tmp->board);
        freeBoard(tmp->board);
        free(tmp);
        tmp = tmp1;
    }
}

void printList(DubList* list){
    Node * node ;
    Node* tmp;

    node = list->curr;
    if ( node == NULL){
        printf("list is empty \n");
    } else {
        printf("FOREWARD:\n");
        printf("-----------\n");
        tmp = node;
        while (tmp != NULL )
        {
            printf("node addr is  %p \n",(void*)tmp);
            printf("\tnext id is: %p  \n",(void*)tmp->next);
            printf("\tprev id is: %p  \n",(void*)tmp->prev);
            printf("\tboard is: %p  \n",(void*)tmp->board);
            tmp = tmp->next;
        }
        printf("BACKWARDS:\n");
        printf("-----------\n");
        tmp = node;
        while (tmp != NULL )
        {
            printf("node addr is  %p \n",(void*)tmp);
            printf("\tnext id is: %p  \n",(void*)tmp->next);
            printf("\tprev id is: %p  \n",(void*)tmp->prev);
            printf("\tboard is: %p  \n",(void*)tmp->board);
            tmp = tmp->prev;
        }
        printf("done printing \n");

    }
}

int pushToList(DubList* list, Board* boardPtr){

    Node* tmp;
    Node* current;
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->board = boardPtr;
    newNode->next = NULL;
    current = list->curr;

    if (current == NULL)
    {
        /*First element*/
        list->curr = newNode;
        newNode->prev = NULL;
    }   else {
        /*Not First element		*/
        tmp = current->next;
        current->next = newNode;
        newNode->prev = current;
        /*
        printf("print after push: \n");
        printList(list);
        */
        list->curr = newNode;


        freeNextNodes(tmp);

    }

    return 0;
}


int goStepBack(DubList* list){
    Node * node ;
    node = list->curr;

    if (node == NULL ){
        printf("Error: list is empty\n");
        return 0;
    } else if (node->prev == NULL){
        printf("Error: no more undos \n");
        return 0;
    } else {
        list->curr = node->prev;
        return 1;
    }
}
int goStepForward(DubList* list){
    Node * node ;
    node = list->curr;

    if (node == NULL ){
        printf("Error: list is empty\n");
        return 0;
    } else if (node->next == NULL){
        printf("Error: no more Redos \n");
        return 0;
    } else {
        list->curr = node->next;
        return 1;
    }
}

int freeList(DubList* list){
    Node* node = list->curr;
    Node* tmp;

    if (node == NULL){
        return 1;
    }

    /*Go back all the way*/
    tmp = node->prev;

    while (tmp != NULL )
    {
        node = tmp;
        tmp = node->prev;

    }
    /*free all next nodes*/
    list->curr = NULL;
    freeNextNodes(node);
    return 1;
}

/*douly linked list end*/

bool mainLoop(){
    Board* board;
    DubList dlist = {0};
    DubList* moves = &dlist;

    CMD cmd = {0};
    char cmd_text[CMD_MAX_LENGTH + 1] = {0};


    while (true){

        if (!get_command(cmd_text))
        {
            break;
        }

        if (!parse_command(cmd_text, &cmd))
        {
            continue;
        }

        if (cmd.type == UNDO || cmd.type == REDO) {
            /*do your thing*/
        } else {
            if (!do_commands(&cmd, &board)) {
                continue;
            }
            /* command suceeded
            pushToList(moves, creatCopiedBoard(board));
            */
        }
        printBoard(board);

        }



    return true;
}


