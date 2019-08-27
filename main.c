#include <stdio.h>

#include "main_aux.h"


int main(int argc, char *argv[]) { 
	int status = 0;
	printf("Welcome to the Gurobi game ! \n");

	status = play_game();

	printf("exiting\n");

	return status;
}
