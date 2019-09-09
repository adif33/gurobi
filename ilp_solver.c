#include "ilp_solver.h"
#include "gurobi_c.h"

int threeDimTo1d(int i,int j,int k, int dim) {
    return (i*dim*dim + j*dim + k);
}

bool updateSolutionToBoard(Board* board,GRBmodel *model,int dim){
    int i,j,k,tmp,tmp_index,error;
    double dtmp;
    for ( i = 0; i < dim; ++i) {
        for ( j = 0; j < dim; ++j) {
            for ( k = 0; k < dim; ++k) {

                tmp_index = threeDimTo1d(i,j,k,dim);
                error = GRBgetdblattrelement(model, "X", tmp_index, &dtmp);
                if (error){
                    return error;
                }

                if ((int) dtmp == 1){

                    tmp = getCellValue(board,i,j);
                    /*
                    printf("i: %i,j:%i,k:%i val:%i \n",i,j,k,tmp);
                    */
                    if (tmp != k+1 ) {
                        if (tmp != 0){
                            printf("Error: bad solution \n");
                            return 1;
                        }
                        if (setVal(board,i,j,k+1) == false ){
                            printf("Error: problem updating board\n");
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return error;
}



int addConstEachCellSingleValue(GRBmodel *model,int N,int* ind,double* val){
    int k,i,j,error ;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                ind[k] = threeDimTo1d(i,j,k,N);
                val[k] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) return error;
        }
    }

    return 0;
}

int addConstEachRowValue(GRBmodel *model,int N,int* ind,double* val){
    int k,i,j,error ;

    for (k = 0; k < N; k++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = threeDimTo1d(i,j,k,N);
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) return error;
        }
    }

    return 0;
}

int addConstEachColumnValue(GRBmodel *model,int N,int* ind,double* val){
    int k,i,j,error ;

    for (k = 0; k < N; k++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = threeDimTo1d(i,j,k,N);
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) return error;
        }
    }

    return 0;
}

int addConstEachBlockValue(GRBmodel *model,int N,int* ind,double* val,int n, int m){
    int k,i,j,ib,jb,tmp_index,error ;

    for (k = 0;  k<N ; ++k) {
        for (ib = 0; ib < n ;++ib) {
            for (jb = 0;  jb<m ; jb++) {
                tmp_index = 0;
                for (i = ib*m; i <(ib*m + m) ; i++) {
                    for (j = jb*n;  j<(jb*n + n)  ; j++) {
                        /*
                         printf("i:%i,j:%i k:%i,tmp =%i \n",i,j,k,tmp_index);
                         */
                         ind[tmp_index] = threeDimTo1d(i,j,k,N);
                         val[tmp_index] = 1.0;
                         tmp_index +=1;
                     }
                 }
                 error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) return error;
             }

         }
     }

    return 0;
}

int updateLPSolutionToBoard(Board* board,GRBmodel *model,double thres,int dim){
    int i,j,k,error,tmp_index,value;
    double dtmp;
    bool* valid;
    double* weight;
    int* numbers;

    valid = calloc(sizeof(bool),dim +1);
    weight = calloc(sizeof(double),dim+1);
    numbers = calloc(sizeof(int),dim+1);

    for ( i = 0; i < dim + 1; ++i) {
        numbers[i] = i;
    }

    for (i = 0; i < dim; ++i) {
        for ( j = 0; j < dim; ++j) {
            if (getCellValue(board,i,j) == 0 ){
                detectLegalValues(board,i,j,valid);

                for (k = 0; k < dim; ++k) {
                    tmp_index = threeDimTo1d(i,j,k,dim);
                    error = GRBgetdblattrelement(model, "X", tmp_index, &dtmp);
                    if (error){
                        return error;
                    }
                    if (dtmp > thres){
                        weight[k+1] = dtmp*(int)valid[k+1];
                    } else {
                        weight[k+1] = 0;
                    }

                }
                /*printf("i:%i, j: %i k: %i val: %f, valid %d \n",i,j,k,dtmp,valid[k+1]);*/
                value = chooseRandomNumberByWeight(numbers,weight,dim+1);
                if (!setVal(board,i,j,value)){
                    return 1;
                }
            }
        }
    }

    free(valid);
    free(weight);
    free(numbers);
    return 0;
}



 /*
  * Treats every cell with value != 0 as fixed
  * regardless to the fixed bool value
  * because it will run on a newly created boards
  * */
void solveBoard(Board* board,Solution* sol){

    /* Create an empty model */
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;

    char     *cursor;
    char      *namestorage;
    int N = board->N;
    int n = board->n;
    int m = board->m;
    int i ,j, k,tmp_lower_bound, tmp_index;
    double* lb;
    double* val;
    int* ind;
    char* vtype ;
    char** names;
    int error = 0;
    int       optimstatus;
    char type;
    sol->error =0;

    if (sol->stat == guess){
        type = GRB_CONTINUOUS ;
    } else {
        type = GRB_BINARY ;
    }


    namestorage =   calloc((2*3 + 6)*(N*N*N), sizeof(char) );
    lb          =   calloc(N*N*N, sizeof(double));
    vtype       =   calloc(N*N*N, sizeof(char));
    names       =   calloc(N*N*N, sizeof(char*));
    val         =   calloc(N, sizeof(double));
    ind         =   calloc(N, sizeof(int));

    /* Create an empty model */
    cursor = namestorage;
    for (i = 0; i < N; i++) {
        for (j = 0; j <N ; j++) {
            for (k = 0; k <N ; k++) {

                if (getCellValue(board,i,j) - 1  == k  ){
                    /*
                    printf("i=%i,j=%i,k=%i \n",i,j,k);
                    printf("index is: %i \n",threeDimTo1d(i,j,k,N));
                    */
                    tmp_lower_bound =1;
                } else {
                    tmp_lower_bound =0;
                }
                tmp_index = threeDimTo1d(i,j,k,N) ;
                lb[tmp_index]   = tmp_lower_bound;
                vtype[tmp_index]= type;

                names[tmp_index] = cursor;
                sprintf(names[tmp_index], "x[%d,%d,%d]", i, j, k+1);
                cursor += strlen(names[tmp_index]) + 1;

            }
        }
    }

    /* Create environment */
    error = GRBloadenv(&env, "sudoku.log");
    if (error) goto QUIT;

     /*Turm off printing to console*/
     error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
     if (error) goto QUIT;

    /* Create new model */
    error = GRBnewmodel(env, &model, "sudoku", N*N*N, NULL, lb, NULL,vtype, names);
    if (error) goto QUIT;

    /* Each cell gets a value     */
    addConstEachCellSingleValue(model,N,ind,val);

    /* Each value must appear once in each row    */
    addConstEachRowValue(model,N,ind,val);

    /* Each value must appear once in each column */
    addConstEachColumnValue(model,N,ind,val);

    /* Each value must appear once in each subgrid */
    addConstEachBlockValue(model,N,ind,val,n,m);



    /* Optimize model */
    error = GRBoptimize(model);
    if (error) goto QUIT;

    /* Write model to 'sudoku.lp' */
    error = GRBwrite(model, "sudoku.lp");
    if (error) goto QUIT;

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    if (optimstatus == GRB_OPTIMAL) {
        /*
        printf("Optimal objective\n");
         */
        sol->solved = true;
        if (sol->stat == generate){
            error = updateSolutionToBoard(board,model,N);
            if (error) goto QUIT;
        } else if(sol->stat == guess){
            error = updateLPSolutionToBoard(board,model,sol->thres,N);
            if (error) goto QUIT;
        }
    }
    else
    {
        /*
        printf("Model is infeasible or unbounded\n");
         */
        sol->solved = false;

    }


    QUIT:
    /* Error reporting */

    if (error) {
     printf("ERROR: %s\n", GRBgeterrormsg(env));
     sol->error = error;
    }

    /* Free model */
    GRBfreemodel(model);

    /* Free environment */
    GRBfreeenv(env);

    /* Free callocs */
    free(namestorage);
    free(lb);
    free(vtype);
    free(names);
    free(val);
    free(ind);


}

bool validateBoard(Board* board){

    Solution sol = {0};

    solveBoard(board,&sol);

    if (sol.error == 0){
        if (sol.solved){
            printf("Board is valid ! \n");
        } else {
            printf("Board is not valid ! \n");
        }
        return true;
    }
    return false;

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

    /* print legal values:
     *
    printf("legal values(%i):  \n",count);

    for ( i = 1; i < N+1 ; ++i) {
        if (values[i] == true){
            printf(" %i ,",i);
        }
    }
    printf("\n");
     */
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

bool generateBoard(Board** board_ptr,int cellsToFill, int cellsToRemove ){
    int i,emptyCells, removableCells;
    Board* board ;
    Board* copy ;
    Solution sol = {0};
    sol.stat = generate;
    board = *board_ptr;
    srand(time(NULL));

    emptyCells =countEmptyCells(board);

    if ( emptyCells < cellsToFill ){
        printf("Error: number of empty cells is insufficient \n");
        return false;
    }

    removableCells = countRemovableCells(board);
    if ( removableCells < cellsToRemove ){
        printf("Error: number of removable cells is insufficient \n");
        return false;
    }

    for (i = 0; i < GENERATE_CMD_NUM_RETRYS ; ++i) {

        copy = creatCopiedBoard(board);
        printf("created copy %p \n",(void*)copy);

        if (!tryToFillBoard(copy,cellsToFill)){
            freeBoard(copy);
            continue;
        }

        solveBoard(copy,&sol );
        if (sol.error){
            return false;
        }
        if (!sol.solved){
            freeBoard(copy);
            continue;
        }
        break;
    }

    if (i == GENERATE_CMD_NUM_RETRYS ){
        printf("Error: generation failed after %i times \n",GENERATE_CMD_NUM_RETRYS);
        return false;
    }

    if (!removeYRandomCells(copy,cellsToRemove)){
        printf("Error: failed removing cells \n");
        return false;

    }
    freeBoard(board);
    *board_ptr = copy;

    return true;
}

bool guessBoard(Board* board, double threshold){
    Solution sol = {0};
    sol.stat = guess ;
    sol.thres = threshold;
    srand(time(NULL));

    solveBoard(board,&sol);
    if (sol.error){
        return false;
    }
    if (!sol.solved){
        printf("Error: could not solve board \n");
    }
    return true;


}
