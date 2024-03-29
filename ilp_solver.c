#include "ilp_solver.h"
#include "gurobi_c.h"

/*
 * recieves an array of ins and their weight and then chooses random number
 * and returns the value according to the weights
 */
int chooseRandomNumberByWeight(int* numbers,double* weights,int size){
    int i,random;
    int limit ;
    int factor = 100;
    double sum =0;

    for ( i = 0; i < size; ++i) {
        /*printf("number is: %i weight is:%f \n",numbers[i],weights[i]);*/
        sum += weights[i];
    }
    if (sum == 0){
        /*all weights are zero*/
        return 0;

    }
    limit = factor * sum;
    random = rand() % (limit ) + 1 ;

    /*printf("limit is: %i random is: %i \n",limit,random);*/

    for ( i = 0; i < size ; ++i) {
        if (random < weights[i]*factor ){
            return numbers[i];
        }
        random -= weights[i]*factor;
    }
    return 0;
}


int countEmptyCells(Board* board){
    int  i,j;
    int N = board->N;
    int count =0;

    for ( i = 0; i <N ; ++i) {
        for (j = 0; j <N ; ++j) {
            if (getCellValue(board,i,j) == 0){
                count +=1;
            }
        }
    }
    return count;
}

int countRemovableCells(Board* board){
    int  i,j;
    int N = board->N;
    int count =0;

    for ( i = 0; i <N ; ++i) {
        for (j = 0; j <N ; ++j) {
            if (!isCellFixed(board,i,j)){
                count +=1;
            }
        }
    }
    return count;
}
bool setRandomLegalValue(Board* board,int row, int col ){
    bool* values;
    int i ,index,count;
    int N = board->N;
    count = N;


    values = calloc(N + 1, sizeof(bool));

    count = detectLegalValues(board,row,col,values);

    /*
    printf("legal values(%i):  \n",count);

    for ( i = 1; i < N+1 ; ++i) {
        if (values[i] == true){
            printf(" %i ,",i);
        }
    }
    printf("\n");*/

    if (count == 0){
        /*
        printf("Error: No legal values available \n");
         */
        return false;
    }

    index = rand() % (count ) + 1 ;

    for ( i = 0; i < N+1 ; ++i) {
        if (values[i] == true){
            index -=1;
            if (index == 0){
                setVal(board,row,col,i);
                break;
            }
        }
    }

    /*
    printf("index is %i \n",index);
    printBoard(board);
    */
    free(values);
    return true;
}

bool setNthEmptyCell(Board* board,int n) {
    int i, j;
    int N = board->N;
    int count = 0;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (getCellValue(board, i, j) == 0) {
                count += 1;
                if (count == n) {
                    return setRandomLegalValue(board,i,j);
                }
            }
        }
    }
    return false;
}

bool removeNthCell(Board* board,int n) {
    int i, j, val;
    bool fixed ;
    int N = board->N;
    int count = 0;

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            val =getCellValue(board, i, j) ;
            fixed = isCellFixed(board,i,j);
            if ( val != 0 && fixed == false ) {
                count += 1;
                if (count == n) {
                    return setVal(board,i,j,0);
                }
            }
        }
    }
    return false;
}

bool tryToFillBoard(Board* board, int cellsToFill){
    int i, emptyCells, index ;
    emptyCells =countEmptyCells(board);

    for ( i = 0; i < cellsToFill; ++i) {
        index = rand() % (emptyCells ) + 1 ;
        if (!setNthEmptyCell(board,index)){
            return false;
        }
        emptyCells -=1;
    }
    return true;

}


bool removeYRandomCells(Board* board,int cellsToRemove){
    int  i, removableCells, index ;
    removableCells =countRemovableCells(board);

    for ( i = 0; i < cellsToRemove; ++i) {
        index = rand() % (removableCells ) + 1 ;
        if (!removeNthCell(board,index)){
            return false;
        }
        removableCells -=1;
    }
    return true;

}
/*
 * gets a board and returns the gurobi env
 * adds variables and constraints in a very smart way !
 * the variable type is given too as vtype
 * if this is a LP (not GRB_BINARY) then uses a  target function
 */
int getNewILPModel(Board* board,GRBmodel** model_ptr ,GRBenv** env_ptr,char vtype){
    int N,i,j,k,error,value;
    bool* valid;
    double weight;


    /* Create an empty model */
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;

    char name[VAR_NAME_MAX_SIZE];
    N = board->N;
    valid = calloc(N+1, sizeof(bool));

    /* Create environment */
    error = GRBloadenv(&env, "sudoku.log");
    if (error) return error;

    /*Turm off printing to console*/
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) return error;

    /* Create new model */
    error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL,NULL, NULL);
    if (error) return error;

    *env_ptr = env;
    *model_ptr = model;

    for (i = 0; i < N ; ++i) {
        for ( j = 0; j < N; ++j) {
            value = getCellValue(board,i,j);

            if (value == 0){
                weight= (double) detectLegalValues(board,i,j,valid);
                /*printf("i:%i, j:%i weight is: %f \n",i,j,weight);*/
                for (k = 0; k < N; ++k) {
                    if (valid[k+1]){
                        sprintf(name,"x[%d,%d,%d]",i,j,k+1);
                        /*printf("i: %i,j: %i,k: %i, name: %s\n",i,j,k,name);*/
                        if (vtype == GRB_BINARY){
                            weight = 0.0;
                        }
                        error = GRBaddvar(model, 0, NULL, NULL, weight, 0.0, 1.0,vtype, name);
                        if (error) return error;
                    }
                }
            }
        }
    }
    free(valid);
    return error;
}
/*
 * add constrains  - every cell has a single value
 */
int addConstEachCellSingleValueFullBoard(Board* board,GRBmodel *model,int* ind, double* val,int N){
    int i,j,k, error,index,count;
    char name[VAR_NAME_MAX_SIZE];
    count = 0;
    index = -1;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (getCellValue(board,i,j) == 0) {
                count = 0;
                for (k = 0; k < N; ++k) {
                    sprintf(name, "x[%d,%d,%d]", i, j, k + 1);
                    /*printf("i: %i,j: %i,k: %i, name: %s\n",i,j,k,name);*/
                    error = GRBgetvarbyname(model, name, &index);
                    if (error) return error;

                    if (index != -1) {
                        ind[count] = index;
                        count += 1;
                    }

                }
                if (count != 0){
                    error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);
                    if (error) return error;
                }
            }
        }
    }
    return error;
}
/*
 * add constrains  - every row has a single value
 */
int addConstEachRowSingleValueFullBoard(GRBmodel *model,int* ind, double* val,int N){
    int i,j,k, error,index,count;
    char name[VAR_NAME_MAX_SIZE];
    count = 0;
    index = -1;
    error = 0;

    for (i = 0; i < N; i++) {
        for ( k = 0; k <N ; ++k) {
            count = 0;

            for ( j = 0; j < N; ++j) {
                sprintf(name, "x[%d,%d,%d]", i, j, k + 1);
                error = GRBgetvarbyname(model, name, &index);
                if (error) return error;

                if (index != -1) {
                    ind[count] = index;
                    count += 1;
                }
            }
            if (count != 0){
                error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) return error;
            }
        }
    }
    return error;
}
/*
 * add constrains  - every column has a single value
 */
int addConstEachColumnSingleValueFullBoard(GRBmodel *model,int* ind, double* val,int N){
    int i,j,k, error,index,count;
    char name[VAR_NAME_MAX_SIZE];
    count = 0;
    index = -1;

    for (j = 0; j < N; j++) {
        for ( k = 0; k <N ; ++k) {
            count = 0;

            for ( i = 0; i < N; ++i) {
                sprintf(name, "x[%d,%d,%d]", i, j, k + 1);
                error = GRBgetvarbyname(model, name, &index);
                if (error) return error;

                if (index != -1) {
                    ind[count] = index;
                    count += 1;
                }
            }
            if (count != 0){
                error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) return error;
            }
        }
    }
    return error;
}
/*
 * add constrains  - every block has a single value
 */
int addConstEachBlockSingleValueFullBoard(Board* board,GRBmodel *model,int* ind, double* val,int N){
    int i,ib,j,jb,k, error,index,count,n,m;
    char name[VAR_NAME_MAX_SIZE];
    count = 0;
    index = -1;

    n = board->n;
    m = board->m;

    for (k = 0; k < N; ++k) {
        for (ib = 0; ib < n; ++ib) {
            for (jb = 0; jb < m; ++jb) {
                count = 0;
                for ( i = ib*m; i < (ib*m +m) ; ++i) {
                    for (j = jb*n; j < (jb*n + n ) ; ++j) {
                        sprintf(name, "x[%d,%d,%d]", i, j, k + 1);
                        error = GRBgetvarbyname(model, name, &index);
                        if (error) return error;

                        if (index != -1) {
                            ind[count] = index;
                            count += 1;
                        }

                    }
                }
                if (count != 0){
                    error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);
                    if (error) return error;
                }
            }
        }
    }


    return error;
}

/*
 * adds all of the constrains
 */

int addILPConstraints(Board* board,GRBmodel *model){
    int i,error,N ;
    int* ind;
    double* val;

    N = board->N;
    val = calloc(N, sizeof(double));
    ind = calloc(N, sizeof(int));

    for ( i = 0; i < N; ++i) {
        val[i] = 1.0;
    }

    error = addConstEachCellSingleValueFullBoard(board,model,ind,val,N);
    if (error) return error;

    error = addConstEachRowSingleValueFullBoard(model,ind,val,N);
    if (error) return error;

    error = addConstEachColumnSingleValueFullBoard(model,ind,val,N);
    if (error) return error;

    error = addConstEachBlockSingleValueFullBoard(board,model,ind,val,N);
    if (error) return error;



    free(val);
    free(ind);
    return error;

}

int solveBoard(Board* board,GRBmodel** model_ptr ,GRBenv** env_ptr,char vtype){
    int error;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;

    error = getNewILPModel(board,&model,&env,vtype);
    if (error) return error ;

    *env_ptr = env;
    *model_ptr = model;

    /* Update model because its laze */
    error = GRBupdatemodel(model);
    if (error) return error ;

    /* Write model to 'sudoku.lp' */
    error = GRBwrite(model, "sudoku.lp");
    if (error) return error ;

    error = addILPConstraints(board,model);
    if (error) return error ;

    /* Optimize model */
    error = GRBoptimize(model);
    if (error) return error ;

    /* Write model to 'sudoku.lp' */
    error = GRBwrite(model, "sudoku.lp");
    if (error) return error ;


    return error;
}

int updateSolutionToBoard(Board* board,GRBmodel *model){
    int i,j,k,error,N,index;
    double dtmp;
    char name[VAR_NAME_MAX_SIZE];

    N = board->N;

    for ( i = 0; i < N; ++i) {
        for ( j = 0; j <N ; ++j) {
            if (getCellValue(board,i,j) == 0 ){
                for (k = 0; k < N; ++k) {
                    sprintf(name, "x[%d,%d,%d]", i, j, k + 1);
                    /*printf("i: %i,j: %i,k: %i, name: %s\n",i,j,k,name);*/
                    error = GRBgetvarbyname(model, name, &index);
                    if (error) return error;

                    if (index != -1 ){
                        error = GRBgetdblattrelement(model, "X", index, &dtmp);
                        if (error) return error;
                        if ((int) dtmp == 1){
                            if (setVal(board,i,j,k+1) == false ){
                                printf("Error: problem updating board\n");
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return error;
}


bool validateBoard(Board* board){

    int error, optimstatus;
    bool res;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;

    res = true;

    if (countEmptyCells(board) <= 0){
        printf("board is already solved \n");
        return true;
    }

    error = solveBoard(board,&model,&env,GRB_BINARY);
    if (error) goto QUIT;

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        printf(BOARD_IS_SOLVABLE_MSG);
    } else {
        printf(BOARD_IS_NOT_SOLVABLE_MSG);
    }


    QUIT:
    /* Error reporting */
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        res = false;
    }

    GRBfreemodel(model);
    GRBfreeenv(env);

    return res;
}
/*
 * runs in a loop. tries to  fill random valid values,
 * then tries to solve using ILP and then removes random Y cells
 * if falis, returns false and pring relevant error msg
 */
bool generateBoard(Board** board_ptr,int cellsToFill, int cellsToKeep ){
    int i,N,emptyCells,error ,optimstatus, cellsToRemove;
    Board* board ;
    Board* copy ;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;
    bool res;

    res =true;
    board = *board_ptr;
    N = board->N;
    srand(time(NULL));

    emptyCells =countEmptyCells(board);
    cellsToRemove = N*N - cellsToKeep;

    if ( emptyCells < cellsToFill ){
        printf(GENERATE_PARAMETER_X_NOT_ENOUGH_EMPTY_CELLS);
        return false;
    }

    /* NOT POSSIBLE !!!!!!
    removableCells = countRemovableCells(board);
    if ( removableCells < cellsToRemove ){
        printf("Error: number of removable cells is insufficient \n");
        return false;
    }
     */

    for (i = 0; i < GENERATE_CMD_NUM_RETRYS ; ++i) {

        copy = creatCopiedBoard(board);
        /*printf("created copy %p \n",(void*)copy);*/

        if (!tryToFillBoard(copy,cellsToFill)){
            freeBoard(copy);
            continue;
        }

        error = solveBoard(copy,&model,&env,GRB_BINARY);
        if (error) goto QUIT;

        /* Capture solution information */
        error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
        if (error) goto QUIT;

        if (optimstatus == GRB_OPTIMAL) {
            /*printf("Board is solvable ! \n");*/
            error = updateSolutionToBoard(copy,model);
            if (error) goto QUIT;
            break;
        } else {
            /*printf("Board is not solvable ! \n");*/
            freeBoard(copy);
            GRBfreemodel(model);
            GRBfreeenv(env);
            continue;
        }

    }

    if (i == GENERATE_CMD_NUM_RETRYS ){
        printf(GENERATE_FAILED_MAX_RETRYS);
        return false;
    }

    if (!removeYRandomCells(copy,cellsToRemove)){
        printf("Error: failed removing cells \n");
        return false;

    }

    QUIT:
    /* Error reporting */
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        res = false;
    }

    GRBfreemodel(model);
    GRBfreeenv(env);

    freeBoard(board);
    *board_ptr = copy;

    return res;
}
/*
 * update LP solution to the given board according to threshold
 */
int updateLPSolutionToBoard(Board* board,GRBmodel *model,double thres){
    int i,j,k,error,index,value,N;
    double dtmp;
    double* weight;
    int* numbers;
    bool* valid;
    char name[VAR_NAME_MAX_SIZE];

    N = board->N;
    weight = calloc(sizeof(double),N+1);
    numbers = calloc(sizeof(int),N+1);
    valid = calloc(sizeof(bool),N +1);

    for ( i = 0; i < N + 1; ++i) {
        numbers[i] = i;
    }

    for (i = 0; i < N; ++i) {
        for ( j = 0; j < N; ++j) {
            if (getCellValue(board,i,j) == 0 ){

                detectLegalValues(board,i,j,valid);
                memset(weight,0, sizeof(double)*(N+1));
                /*printf("valid: \n",i);
                for ( k = 0; k <N +1 ; ++k) {
                    printf(" value: %i valid  %i,",k,valid[k]);
                }
                printf(" \n",i);*/

                for (k = 0; k < N; ++k) {
                    sprintf(name, "x[%d,%d,%d]", i, j, k + 1);
                    error = GRBgetvarbyname(model, name, &index);
                    if (error) return error;
                    if (index != -1 ){
                        error = GRBgetdblattrelement(model, "X", index, &dtmp);
                        if (error) return error;
                        if (dtmp > thres){
                            weight[k+1] = (double)dtmp*(double)valid[k+1];
                        } else {
                            weight[k+1] = 0;
                        }
                    }
                    /*printf("i: %i,j: %i,k: %i, name: %s,valid: %i,index: %i,dtmp: %f, mult: %f\n",i,j,k,name,(int)valid[k+1],index,dtmp,(double)dtmp*(double)valid[k+1]);*/
                }
                value = chooseRandomNumberByWeight(numbers,weight,N+1);
                if (!setVal(board,i,j,value)){
                    return 1;
                }
            }
        }
    }

    free(weight);
    free(numbers);
    free(valid);
    return error;
}
/*
 * uses LP to solve the board, then if there is a solution
 * shows rhe relevant values according the threshold
 */
bool guessBoard(Board* board, double threshold){
    int error, optimstatus;
    bool res;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;

    res = true;
    srand(time(NULL));

    error = solveBoard(board,&model,&env,GRB_CONTINUOUS);
    if (error) goto QUIT;


    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        /*printf("Board is solvable ! \n");*/
        error =updateLPSolutionToBoard(board,model,threshold);
        if (error) goto QUIT;

    } else {
        printf(BOARD_IS_NOT_SOLVABLE_ERROR);
        res = false;
    }


    QUIT:
    /* Error reporting */
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        res = false;
    }

    GRBfreemodel(model);
    GRBfreeenv(env);

    return res;
}
/*
 * shows the scores for the relavant cell above 0 score
 * according to the model
 */
int showLPSolutionSingleCell(Board* board,GRBmodel *model,int row, int col){
    int k, N, error,index;
    double dtmp;
    char name[VAR_NAME_MAX_SIZE];

    N = board->N;

    printf("Used LP solution to determine the following scores: \n");
    for ( k = 0; k < N ; ++k) {
        sprintf(name, "x[%d,%d,%d]", row, col, k + 1);
        error = GRBgetvarbyname(model, name, &index);

        if (index != -1 ){
            error = GRBgetdblattrelement(model, "X", index, &dtmp);
            if (error) return error;
            if (dtmp > 0 ){
                printf("Value: %i, Score: %f \n",k +1 ,dtmp);
            }
        }
    }

    return 0;
}
/*
 * gives the hing to the given cell according to the gurobi model
 */

int showILPSolutionSingleCell(Board* board,GRBmodel *model,int row, int col){
    int k, N, error,index;
    double dtmp;
    char name[VAR_NAME_MAX_SIZE];

    N = board->N;

    for ( k = 0; k < N ; ++k) {
        sprintf(name, "x[%d,%d,%d]", row, col, k + 1);
        error = GRBgetvarbyname(model, name, &index);

        if (index != -1 ){
            error = GRBgetdblattrelement(model, "X", index, &dtmp);
            if (error) return error;
            if (dtmp > 0 ){
                printf(HINT_SINGLE_CELL_MSG);
                return 0;
            }
        }
    }

    return 1;
}

bool guessHintBoard(Board* board, int row,int col){
    int error, optimstatus;
    bool res;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;

    res = true;

    error = solveBoard(board,&model,&env,GRB_CONTINUOUS);
    if (error) goto QUIT;

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        /*printf("Board is solvable ! \n");*/
        error = showLPSolutionSingleCell(board,model,row,col);
        if (error) goto QUIT;

    } else {
        printf(BOARD_IS_NOT_SOLVABLE_ERROR);
        res= false;
    }


    QUIT:
    /* Error reporting */
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        res = false;
    }

    GRBfreemodel(model);
    GRBfreeenv(env);

    return res;
}
/*
 * prints a hint to user according to the gurobi ILP solution
 */
bool hint(Board* board, int row,int col){
    int error, optimstatus;
    bool res;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;

    res = true;

    error = solveBoard(board,&model,&env,GRB_BINARY);
    if (error) goto QUIT;

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        /*printf("Board is solvable ! \n");*/
        error = showILPSolutionSingleCell(board,model,row,col);
        if (error) goto QUIT;

    } else {
        printf(BOARD_IS_NOT_SOLVABLE_ERROR);
        res = false;
    }


    QUIT:
    /* Error reporting */
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        res = false;
    }

    GRBfreemodel(model);
    GRBfreeenv(env);

    return res;

}
