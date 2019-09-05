#include <stdio.h>

#include "solver.h"



Stack* createStack(int capacity)
{
    Stack *pt = (Stack*)malloc(sizeof(Stack));

    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (StackItem**)malloc(sizeof(StackItem*) * capacity);


    return pt;
}

StackItem* createStackItem(Board* board, int row, int column)
{
    StackItem *pt = (StackItem*)malloc(sizeof(StackItem));

    pt->board = board;
    pt->row = row;
    pt->column = column;

    return pt;
}

int size(Stack *pt)
{
    return pt->top + 1;
}

int isEmpty(Stack *pt)
{
    return pt->top == -1;\
}

int isFull(Stack *pt)
{
    return pt->top == pt->maxsize - 1;
}

void push(Stack *pt, StackItem* x)
{
    if (isFull(pt))
    {
        printf("OverFlow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }

    pt->items[++pt->top] = x;
}

StackItem* peek(Stack *pt)

{
    if (!isEmpty(pt))
        return pt->items[pt->top];
    else
        exit(EXIT_FAILURE);
}

StackItem* pop(Stack *pt)
{
    if (isEmpty(pt))
    {
        exit(EXIT_FAILURE);
    }


    return pt->items[pt->top--];
}


int getNumberOfSolution(Board* board){
    /*BUG: when the first value is zero*/
    Stack* stack = createStack(MAX_STACK_CAPACITY);
    StackItem* curr_stackItem;
    Board* curr_board;
    Board* new_board;
    int row, column, counter, N, value;

    N = board->N;
    counter = 0;
    if(!isCellEmpty(board, 0, 0)){
        new_board = creatCopiedBoard(board);

        curr_stackItem = createStackItem(new_board, 0, 0);
        push(stack, curr_stackItem);
    } else{
        for(value=1; value<=N; value++){
            new_board = creatCopiedBoard(board);
            setVal(new_board, 0, 0, value);
            curr_stackItem = createStackItem(new_board, 0, 0);
            push(stack, curr_stackItem);
        }
    }


    while(!isEmpty(stack)){
        curr_stackItem = pop(stack);

        curr_board = curr_stackItem->board;
        row = curr_stackItem->row;
        column = curr_stackItem->column;

        if ((gotHorizontalDuplicate(curr_board, row, column) ||
             gotVerticalDuplicate(curr_board, row, column) ||
             gotBlockDuplicate(curr_board, row, column))){
            freeBoard(curr_board);
            free(curr_stackItem);

        } else{
            if (row==N-1 && column==N-1){
                counter++;
                freeBoard(curr_board);
                free(curr_stackItem);
                continue;
            }

            column = (column+1)%N;
            if(column==0){
                row += 1;
            }

            if(isCellEmpty(curr_board, row, column)){
                free(curr_stackItem);

                for(value=1; value<=N; value++){
                    new_board = creatCopiedBoard(curr_board);
                    setVal(new_board, row, column, value);
                    curr_stackItem = createStackItem(new_board, row, column);
                    push(stack, curr_stackItem);

                }
                freeBoard(curr_board);

            } else{
                curr_stackItem->row = row;
                curr_stackItem->column = column;
                push(stack, curr_stackItem);
            }
        }


    }

    return counter;
}


int countEmpty(Board* board){
    return 10;
}

void getRandomEmpty(Board* board, int* row, int* column){
    return;
}

int getLegalValues(int* values, Board* board, int row, int column){
    return 10;
}

bool generate(Board* board, int x, int y){
    int empty, count, row, column, size, value, i;
    int* values;
    Board* original_copy;
    time_t t;

    srand((unsigned) time(&t));

    empty = countEmpty(board);
    if (empty<x){
        /*ERRORL print error*/
        return board;
    }



    while(i<1000) {
        original_copy = creatCopiedBoard(board);
        for (count = 0; count < x; count++) {
            getRandomEmpty(original_copy, &row, &column);
            size = getLegalValues(values, original_copy, row, column);
            if(size==0){
                break;
            }
            value = values[rand() % size];
            setVal(original_copy, row, column, value);
        }

        if(count+1!=x){
            break;
        }

        freeBoard(original_copy);
        i++;
    }



    printf("%d\n", rand() % 50);

    return true;

}