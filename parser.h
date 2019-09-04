#ifndef PAR_H_   /* Include guard */
#define PAR_H_

#define CMD_MAX_LENGTH 256

#include "commands.h"


int get_command(char* text_cmd);

bool parse_command(char* text, CMD* command,Board* board);




#endif /* PAR_H_ */
