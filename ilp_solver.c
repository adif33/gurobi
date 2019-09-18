#include "ilp_solver.h"
#include "gurobi_c.h"

/*
 * assumes that all the
 */
int chooseRandomNumberByWeight(int* numbers,double* weights,int size){
    int i,random;
    int limit ;
    int factor = 100;
    double sum =0;

    for ( i = 0; i < size; ++i) {
        printf("number is: %i weight is:%f \n",numbers[i],weights[i]);
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

int getNewILPModel(Board* board,GRBmodel** model_ptr ,GRBenv** env_ptr,char vtype){
    int N,i,j,k,error,value;
    bool* valid;


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
                detectLegalValues(board,i,j,valid);
                for (k = 0; k < N; ++k) {
                    if (valid[k+1]){
                        sprintf(name,"x[%d,%d,%d]",i,j,k+1);
                        /*printf("i: %i,j: %i,k: %i, name: %s\n",i,j,k,name);*/
                        error = GRBaddvar(model, 0, NULL, NULL, 0.0, 0.0, 1.0,vtype, name);
                        if (error) return error;
                    }
                }
            }
        }
    }
    free(valid);
    return error;
}

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
        printf("Board is solvable ! \n");
    } else {
        printf("Board is not solvable ! \n");
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
bool generateBoard(Board** board_ptr,int cellsToFill, int cellsToRemove ){
    int i,emptyCells, removableCells,error ,optimstatus;
    Board* board ;
    Board* copy ;
    GRBmodel *model = NULL;
    GRBenv   *env   = NULL;
    bool res;

    res =true;
    board = *board_ptr;
    srand(time(NULL));

    emptyCells =countEmptyCells(board);

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
        printf("created copy %p \n",(void*)copy);

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
            printf("Board is solvable ! \n");
            error = updateSolutionToBoard(copy,model);
            if (error) goto QUIT;
            break;
        } else {
            printf("Board is not solvable ! \n");
            freeBoard(copy);
            continue;
        }

    }

    if (i == GENERATE_CMD_NUM_RETRYS ){
        printf("Error: generation failed after %i times \n",GENERATE_CMD_NUM_RETRYS);
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
        printf("Board is solvable ! \n");
        error =updateLPSolutionToBoard(board,model,threshold);
        if (error) goto QUIT;

    } else {
        printf("Board is not solvable ! \n");
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
                printf("Used ILP Solution to get the Value: %i \n",k +1);
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
        printf("Board is solvable ! \n");
        error = showLPSolutionSingleCell(board,model,row,col);
        if (error) goto QUIT;

    } else {
        printf("Board is not solvable ! \n");
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
        printf("Board is solvable ! \n");
        error = showILPSolutionSingleCell(board,model,row,col);
        if (error) goto QUIT;

    } else {
        printf("Board is not solvable ! \n");
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
