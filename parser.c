#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"


/*
 * return 0 if EOF is reached, if the command is longer
 * than CMD_MAX_LENGTH it returns invalid  command
 */

int get_command(char* text_cmd){
    /* get command from the user */
    char* ptr;
    int len;
    char c;

    printf(ENTER_COMMAND_BANNER);

    ptr = fgets(text_cmd, CMD_MAX_LENGTH +1  , stdin);
    if (ptr == NULL){
        return 0;
    }

    len = strlen(text_cmd);
    c = text_cmd[len-1];

    if (c == '\n' || len != CMD_MAX_LENGTH )
    {
        return 1;
    }
    /* read until \n FML*/
    while  (ptr != NULL && c != '\n' ){
        ptr = fgets(text_cmd, CMD_MAX_LENGTH +1 , stdin);
        len = strlen(text_cmd);
        c = text_cmd[len-1];

    }
    if (ptr == NULL){
        return 0;
    }
    text_cmd[0] = 97 ;
    text_cmd[1] = 0 ;
    return 1;

}

bool parse_command(char* text, CMD* command,Board* board){
    /* parse the command to the CMD format */
    char* par;
    int i =0 ;
    int argsNum = 0;
    bool wrongMode = false;

    char* type ;

    type = strtok(text, " \t\r\n");
    if (type == NULL) {
        command->type = SKIP;
        return 1;
    } else if (strcmp(type, "solve") == 0){
        command->type = SOLVE;
        argsNum = 1;
    } else if (strcmp(type, "edit") == 0){
        command->type = EDIT;
        argsNum = 1;
    } else if (strcmp(type, "mark_errors") == 0){
        command->type = MARK_ERRORS;
        argsNum = 1;
        if (board->curr_mode != solve ){
            wrongMode = true;
        }
    } else if (strcmp(type, "print_board") == 0){
        command->type = PRINT_BOARD;
        argsNum = 0;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "set") == 0) {
        command->type = SET;
        argsNum = 3;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "validate") == 0) {
        command->type = VALIDATE;
        argsNum = 0;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "guess") == 0) {
        command->type = GUESS;
        argsNum = 1;
        if (board->curr_mode != solve ){
            wrongMode = true;
        }
    } else if (strcmp(type, "generate") == 0) {
        command->type = GENERATE;
        argsNum = 2;
        if (board->curr_mode != edit ){
            wrongMode = true;
        }
    } else if (strcmp(type, "undo") == 0) {
        command->type = UNDO;
        argsNum = 0;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "redo") == 0) {
        command->type = REDO;
        argsNum = 0;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "save") == 0) {
        command->type = SAVE;
        argsNum = 1;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "hint") == 0) {
        command->type = HINT;
        argsNum = 2;
        if (board->curr_mode != solve ){
            wrongMode = true;
        }
    } else if (strcmp(type, "guess_hint") == 0) {
        command->type = GUESS_HINT;
        argsNum = 2;
        if (board->curr_mode != solve ){
            wrongMode = true;
        }
    } else if (strcmp(type, "num_solutions") == 0) {
        command->type = NUM_SOLUTIONS;
        argsNum = 0;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "autofill") == 0) {
        command->type = AUTOFILL;
        argsNum = 0;
        if (board->curr_mode != solve ){
            wrongMode = true;
        }
    } else if (strcmp(type, "reset") == 0) {
        command->type = RESET;
        argsNum = 0;
        if (board->curr_mode == init ){
            wrongMode = true;
        }
    } else if (strcmp(type, "exit") == 0) {
        command->type = EXIT;
        argsNum = 0;
    } else {
        printf(INVALID_CMD_ERROR);
        return false;

    }
    if (wrongMode){
        printf(INVALID_CMD_WRONG_MODE_ERROR);
        return false;
    }

    par = strtok(NULL," \n");
    while( par != NULL && i <= 3) {
        /*
        printf("par is: %s\n",par );
        */
        switch(i) {
            case 0 :
                command->x = par;
                par = strtok(NULL," \n");
                i++;
                continue;
            case 1 :
                command->y = par;
                par = strtok(NULL," \n");
                i++;
                continue;
            case 2 :
                command->z = par;
                par = strtok(NULL," \n");
                i++;
                continue;
            case 3:
                i++;
                break;
        }

    }
    /*
    printf("type is : %s\n",type );
    printf("i is : %i \n",i );
    printf("args mun is : %i\n",argsNum);
    */
    if (i != argsNum)
    {
        if (command->type == EDIT && i ==0 )
        {
            return 1;
        } else if (i < argsNum)
        {
            switch (i)
            {
                case 0 :
                    printf(PARAMETER_X_MISSING_ERROR);
                    break;
                case 1 :
                    printf(PARAMETER_Y_MISSING_ERROR);
                    break;
                case 2 :
                    printf(PARAMETER_Z_MISSING_ERROR);
            }
        } else {
            printf(TOO_MANY_PARAMS_ERROR);
        }
        return 0;

    }
    return 1;
}

