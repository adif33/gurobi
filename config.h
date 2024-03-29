#ifndef CONF_H_   /* Include guard */
#define CONF_H_

/*
 * General
 */
#define GAME_BEGINNING_BANNER "~~~~WELCOME~~~~\n\n"
#define GAME_ENDING_BANNER "EXITING\n"

/*
 * Parser related
 */

#define ENTER_COMMAND_BANNER "Enter command:\n"


#define PARAMETER_X_MISSING_ERROR "ERROR: Parameter x is missing\n"
#define PARAMETER_Y_MISSING_ERROR "ERROR: Parameter y is missing\n"
#define PARAMETER_Z_MISSING_ERROR "ERROR: Parameter z is missing\n"




#define TOO_MANY_PARAMS_ERROR "ERROR: too many arguments\n"

#define INVALID_CMD_ERROR "ERROR: invalid command \n"

#define INVALID_CMD_WRONG_MODE_ERROR "ERROR: invalid command (wrong mode)\n"


#define WRONG_MODE_ERROR "ERROR: invalid command (wrong mode)\n"

#define PARAMETER_X_CONVERSION_ERROR "ERROR: X not in range %i-%i\n",0,board->N
#define PARAMETER_Y_CONVERSION_ERROR "ERROR: Y not in range %i-%i\n",0,board->N
#define PARAMETER_Z_CONVERSION_ERROR "ERROR: Z not in range %i-%i\n",0,board->N



#define PARAMETER_X_NOT_IN_RANGE_ERROR "ERROR: X not in range %i-%i\n",1,board->N
#define PARAMETER_Y_NOT_IN_RANGE_ERROR "ERROR: Y not in range %i-%i\n",1,board->N
#define PARAMETER_Z_NOT_IN_RANGE_ERROR "ERROR: Z not in range %i-%i\n",0,board->N

#define PARAMETER_X_ILLEGAL_ERROR "ERROR: x value is not possible\n"
#define PARAMETER_Y_ILLEGAL_ERROR "ERROR: y value is not possible\n"
#define PARAMETER_Z_ILLEGAL_ERROR "ERROR: z value is not possible\n"

/*
 * Erroneous board
 */
#define ERRONEOUS_BOARD_ERROR "ERROR: Board is erroneous\n"

/*
 * GUROBI related
 */
#define GENERATE_PARAMETER_X_NOT_IN_RANGE_ERROR "ERROR: X not in range %i-%i\n",0,board->N*board->N
#define GENERATE_PARAMETER_Y_NOT_IN_RANGE_ERROR "ERROR: Y not in range %i-%i\n",0,board->N*board->N

#define GENERATE_PARAMETER_X_NOT_ENOUGH_EMPTY_CELLS "ERROR: X not in range %i-%i\n",0,emptyCells
#define GENERATE_FAILED_MAX_RETRYS "ERROR: cant generate. tried %i times\n",GENERATE_CMD_NUM_RETRYS


#define PARAMETER_X_NOT_DOUBLE_ERROR "ERROR: x should be double\n"
#define FIXED_CELL_ERROR "ERROR: Cell is fixed\n"
#define NOT_EMPTY_ERROR "ERROR: Cell is not empty!!\n"

#define GENERATE_CMD_NUM_RETRYS 1000
#define VAR_NAME_MAX_SIZE 12  /* 2*3 + 6  */

#define BOARD_IS_SOLVABLE_MSG "The board is solvable!\n"
#define BOARD_IS_NOT_SOLVABLE_MSG "The board is not solvable!\n"
#define BOARD_IS_NOT_SOLVABLE_ERROR "ERROR: The board is not solvable!\n"
#define HINT_SINGLE_CELL_MSG "Hint: set cell (%i,%i) to: %i\n",col+1,row+1,k +1

/*loader confs*/
#define FILE_OPENING_ERROR "ERROR: In opening file\n"
#define BOARD_FORMAT_ERROR "ERROR: Wrong board format\n"
#define NOT_EMPTY_ERROR "ERROR: Cell is not empty!!\n"

/*is solved*/

#define SOLVED_MESSAGE "Board is solved!!\n"
#define NOT_SOLVED_MESSAGE "You call this solved? Try again...\n"

/*autofill*/

#define CHANGE_CELL_VALUE_FORMAT "Cell (%d,%d) set to %d\n"

#define DELTA_PRINT "Cell (%d,%d) changed from %d to %d\n"

#endif /* CONF_H_ */