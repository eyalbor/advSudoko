CC = gcc
OBJS = main.o main_aux.o game.o solver.o parser.o list.o stack.o ADT_Error.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@
all: $(OBJS)
	$(CC) $(COMP_FLAG) $(OBJS) $(GUROBI_LIB) -o $(EXEC)
main.o: main.c macro.h bool.h ADT_Error.h main_aux.h game.h parser.h
	$(CC) $(COMP_FLAG) -c $*.c
main_aux.o: main_aux.c bool.h macro.h ADT_Error.h main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h game.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h solver.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h main_aux.h game.h solver.h 
	$(CC) $(COMP_FLAG) -c $*.c
list.o: list.c list.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
