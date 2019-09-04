#ifndef PAR_H_   /* Include guard */
#define PAR_H_

#define CMD_MAX_LENGTH 256

#include "commands.h"


int get_command(char* text_cmd);

int parse_command(char* text, CMD* command);






#endif /* PAR_H_ */
