#ifndef PAR_H_   /* Include guard */
#define PAR_H_

#define CMD_MAX_LENGTH 256

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
    char* x;
    char* y;
    char* z;

} CMDArg;

typedef struct {
    CMDType type;
    CMDArg args;
} CMD;

void get_n_m(int *n,int*m);

int get_command(char* text_cmd);

int parse_command(char* text, CMD* command);






#endif /* PAR_H_ */
