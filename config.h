#ifndef CONF_H_   /* Include guard */
#define CONF_H_

/*
 * General
 */
#define GAME_BEGINNING_BANNER "Sudoku\n------\n"
#define GAME_ENDING_BANNER "Exiting...\n"

/*
 * Parser related
 */

#define ENTER_COMMAND_BANNER "Enter your command:\n"

/*
#define PARAMETER_X_MISSING_ERROR "Error: Parameter x is missing\n"
#define PARAMETER_Y_MISSING_ERROR "Error: Parameter y is missing\n"
#define PARAMETER_Z_MISSING_ERROR "Error: Parameter z is missing\n"
 */
#define PARAMETER_X_MISSING_ERROR "ERROR: invalid command\n"
#define PARAMETER_Y_MISSING_ERROR "ERROR: invalid command\n"
#define PARAMETER_Z_MISSING_ERROR "ERROR: invalid command\n"



#define TOO_MANY_PARAMS_ERROR "Error: too many arguments\n"

#define INVALID_CMD_ERROR "Error: invalid command \n"
#define INVALID_CMD_WRONG_MODE_ERROR "Error: invalid command (wrong mode) \n"




#define PARAMETER_X_CONVERSION_ERROR "Error: value not in range %i-%i\n",0,board->N
#define PARAMETER_Y_CONVERSION_ERROR "Error: value not in range %i-%i\n",0,board->N
#define PARAMETER_Z_CONVERSION_ERROR "Error: value not in range %i-%i\n",0,board->N

/*
 *
#define PARAMETER_X_CONVERSION_ERROR "Error: Parameter x is not an integer\n"
#define PARAMETER_Y_CONVERSION_ERROR "Error: Parameter y is not an integer\n"
#define PARAMETER_Z_CONVERSION_ERROR "Error: Parameter z is not an integer\n"
 */



#define PARAMETER_X_NOT_IN_RANGE_ERROR "Error: value not in range %i-%i\n",0,board->N
#define PARAMETER_Y_NOT_IN_RANGE_ERROR "Error: value not in range %i-%i\n",0,board->N
#define PARAMETER_Z_NOT_IN_RANGE_ERROR "Error: value not in range %i-%i\n",0,board->N

#define PARAMETER_X_ILLEGAL_ERROR "Error: Parameter x value is illegal\n"
#define PARAMETER_Y_ILLEGAL_ERROR "Error: Parameter y value is illegal\n"
#define PARAMETER_Z_ILLEGAL_ERROR "Error: Parameter z value is illegal\n"

/*
 * Erroneous board
 */
#define ERRONEOUS_BOARD_ERROR "Error: Board is erroneous\n"

/*
 * Generate + guess related
 */

#define PARAMETER_X_NOT_DOUBLE_ERROR "Error: Parameter x is not a double\n"
#define FIXED_CELL_ERROR "Error: Cell is fixed\n"
#define NOT_EMPTY_ERROR "Error: Cell is not empty\n"

#define GENERATE_CMD_NUM_RETRYS 3
#define VAR_NAME_MAX_SIZE 12  /* 2*3 + 6  */

/*loader confs*/
#define FILE_OPENING_ERROR "ERROR: in opening file\n"
#define BOARD_FORMAT_ERROR "Error: wrong board format\n"
#define NOT_EMPTY_ERROR "Error: Cell is not empty\n"



#endif /* CONF_H_ */