CC = gcc
OBJS = main.o logic.o parser.o solver.o game.o main_aux.o loader.o ilp_solver.o commands.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror  -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all:$(EXEC)

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
	$(CC) $(COMP_FLAG) -c $*.c
logic.o: logic.c logic.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
