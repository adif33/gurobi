#include <stdio.h>

#include "main_aux.h"


int main() {
    int status = 0;
    printf("Welcome to the Gurobi game ! \n");

    status = mainLoop();

    printf("exiting\n");

    return status;
}