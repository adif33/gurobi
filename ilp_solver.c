#include <stdio.h>
#include <string.h>

#include "game.h"
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
                    printf("i: %i,j:%i,k:%i val:%i \n",i,j,k,tmp);

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



 /*
  * Treats every cell with value != 0 as fixed
  * regardless to the fixed bool value
  * because it will run on a newly created boards
  * */
Board* solveBoard(Board* board){

    /* Create an empty model */
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;

    char     *cursor;
    char      *namestorage;
    int N = board->N;
    int n = board->n;
    int m = board->m;
    int i ,j, k,ib,jb,tmp_lower_bound, tmp_index;
    double* lb;
    double* val;
    int* ind;
    char* vtype ;
    char** names;
    int error = 0;
    int       optimstatus;
    double    objval;

    printf("solve board\n");
    printf("n is : %i, m is: %i \n",board->n,board->m);

    printf("size %i \n",(2*3 + 6)*(N*N*N));

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
                vtype[tmp_index]= GRB_BINARY;

                names[tmp_index] = cursor;
                sprintf(names[tmp_index], "x[%d,%d,%d]", i, j, k+1);
                cursor += strlen(names[tmp_index]) + 1;

            }
        }
    }

    /* Create environment */
    error = GRBloadenv(&env, "sudoku.log");
    if (error) goto QUIT;


    /* Create new model */
    error = GRBnewmodel(env, &model, "sudoku", N*N*N, NULL, lb, NULL,vtype, names);
    if (error) goto QUIT;


    /* Each cell gets a value */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                ind[k] = threeDimTo1d(i,j,k,N);
                val[k] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) goto QUIT;
        }
    }

    /* Each value must appear once in each row */

    for (k = 0; k < N; k++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = threeDimTo1d(i,j,k,N);
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) goto QUIT;
        }
    }

    /* Each value must appear once in each column */
    for (k = 0; k < N; k++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = threeDimTo1d(i,j,k,N);
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) goto QUIT;
        }
    }

    /* Each value must appear once in each subgrid */
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
                 if (error) goto QUIT;
             }

         }
     }




    /* Optimize model */

    error = GRBoptimize(model);
    if (error) goto QUIT;

    /* Write model to 'sudoku.lp' */

    error = GRBwrite(model, "sudoku.lp");
    if (error) goto QUIT;

    /* Capture solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) goto QUIT;

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) goto QUIT;

    printf("\nOptimization complete\n");
    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        error = updateSolutionToBoard(board,model,N);
        if (error) goto QUIT;
    }

    else if (optimstatus == GRB_INF_OR_UNBD)
     printf("Model is infeasible or unbounded\n");
    else
     printf("Optimization was stopped early\n");
    printf("\n");

    QUIT:
    /* Error reporting */

    if (error) {
     printf("ERROR: %s\n", GRBgeterrormsg(env));
     return NULL;
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



    return board;

}
