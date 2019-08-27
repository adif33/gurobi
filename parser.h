#ifndef PAR_H_   /* Include guard */
#define PAR_H_

#define CMD_MAX_LENGTH 256

typedef enum cmdtype {
    SET,
    HINT,
    VALIDATE,
    RESTART,
    EXIT,
    SKIP} CMDType;

typedef struct {
    char* param0;
    char* param1;
    char* param2;

} CMDArg;

typedef struct {
    CMDType type;
    CMDArg args;
} CMD;

void get_n_m(int *n,int*m);

int get_command(char* text_cmd);

int parse_command(char* text, CMD* command);






#endif /* PAR_H_ */
