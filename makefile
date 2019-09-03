CC = gcc
OBJS = main.o logic.o parser.o solver.o game.o main_aux.o loader.o ilp_solver.o commands.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra  -pedantic-errors
GUROBI_COMP = -I/opt/gurobi811/linux64/include
GUROBI_LIB = -L/opt/gurobi811/linux64/lib -lgurobi81

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c main.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h
	$(CC) $(COMP_FLAG) -c $*.c
main_aux.o: main_aux.c main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c
loader.o: loader.c loader.h
	$(CC) $(COMP_FLAG) -c $*.c
ilp_solver.o: ilp_solver.c ilp_solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
commands.o: commands.c commands.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
