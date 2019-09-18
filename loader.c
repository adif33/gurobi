#include "loader.h"

bool saveBoard(Board* board, char* path){
    FILE* fp;
    char* text;

    fp = fopen (path, "w+");
    if (fp == NULL)
    {
        printf("ERROR: in opening file\n");
        return false;
    }

    text = createSavedBoardText(board);
    fprintf(fp, "%s", text);

    fclose(fp);
    free(text);

    return true;

}

char* createSavedBoardText(Board* board){
    int len, row, column;
    char* text;
    Cell* curr_cell;


    len = 0;
    text = (char*)malloc(DEFAULT_TEXT_LENGTH);
    len += sprintf(text+len, "%d %d\n", board->n, board->m);
    for(row=0; row<board->N; row++){
        for(column=0; column<board->N; column++){
            curr_cell = getCell(board, row, column);
            if (curr_cell->fixed){
                len += sprintf(text+len, "%d.", curr_cell->value);
            } else{
                len += sprintf(text+len, "%d", curr_cell->value);
            }
            if (column!=board->N-1){
                len += sprintf(text+len, " ");
            }
        }
        if (row!=board->N-1) {
            len += sprintf(text + len, "\n");
        }
    }

    return text;
}

bool isPossibleSetInput(Board* board, int input){
    return input<=board->N;
}



Board* loadBoard(char* path){
    FILE * fd;
    int n, m, N, row, column, input, string_input_len;
    char c;
    char* string_input;
    Board* board;



    fd = fopen(path, "r");

    if (fd == NULL)
    {
        printf("ERROR: in opening file\n");
        return NULL;
    }


    fscanf(fd, "%d %d\n", &m, &n);

    row = 0;
    column = 0;
    board = createEmptyBoard(n, m);
    N = board->N;

    string_input = (char*)malloc(DEFAULT_MATRIX_TEXT_SIZE);
    string_input[0] = '\0';

    while ((c = (char)fgetc(fd)) != EOF  && (row!=N) && (column!=N*N))
    {
        if (c==' ' || c=='\t' || c=='\n'||c=='.'){

            if (strlen(string_input)==0){
                continue;

            } else{
                input = atoi(string_input);

                if (isPossibleSetInput(board, input)){
                    setVal(board, row, column%N, input);
                    if(c=='.'){ /*possible bug. not supporting multiple '.' and such..*/
                        setFixed(board, row, column%N, true);
                    }
                    column++;

                    if(column%N==0){
                        row++;
                    }
                    string_input[0] = '\0';

                } else{
                    /*ERROR!!!!*/
                    printf("ERROR2!!!\n");
                    return NULL;
                }
            }
        } else{

            if (isdigit(c)){ /*is digit*/
                string_input_len = strlen(string_input);
                string_input[string_input_len] = c;
                string_input[string_input_len+1] = '\0';

            } else{
                /*Error!!!*/
                printf("ERROR3!!!\n");
                return NULL;
            }

        }

    }
    free(string_input);
    fclose(fd);
    /*printf("inside: %p\n", board);*/

    return board;
}

