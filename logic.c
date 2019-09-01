
#include "logic.h"





void setErroneous(Board* board){
    int row, column, val;
    for(row=0; row<board->N; row++){
        for(column=0; column < board->N; column++){
            val = board->cells[row][column].value;
            if ((gotHorizontalDuplicate(board, row, column) ||
            gotVerticalDuplicate(board, row, column) ||
            gotBlockDuplicate(board, row, column)) && (val != 0)){
                board->cells[row][column].erroneous = true;
            } else{
                board->cells[row][column].erroneous = false;
            }
        }
    }
}


bool isBoardSolved(Board* board){
    int N, row, column;
    Cell* curr_cell;

    setErroneous(board);



    N = board->N;

    for(row=0; row<N; row++){
        for(column=0; column<N; column++){
            curr_cell = getCell(board, row, column);
            if (curr_cell->erroneous || (curr_cell->value==0)){
                return false;
            }

        }
    }
    return true;

}


Stack* createStack(int capacity)
{
    Stack *pt = (Stack*)malloc(sizeof(Stack));

    pt->maxsize = capacity;
    pt->top = -1;
//    pt->items = (int*)malloc(sizeof(StackItem) * capacity);
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
    return pt->top == -1;	// or return size(pt) == 0;
}

int isFull(Stack *pt)
{
    return pt->top == pt->maxsize - 1;	// or return size(pt) == pt->maxsize;
}

//void push(Stack *pt, StackItem x)
void push(Stack *pt, StackItem* x)
{
    if (isFull(pt))
    {
        printf("OverFlow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }

    pt->items[++pt->top] = x;
}

//StackItem peek(Stack *pt)
StackItem* peek(Stack *pt)

{
    if (!isEmpty(pt))
        return pt->items[pt->top];
    else
        exit(EXIT_FAILURE);
}

//StackItem pop(Stack *pt)
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
                printBoard(curr_board);
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







