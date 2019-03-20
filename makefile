CC = gcc
OBJS = main.o main_aux.o game.o solve.o parcer.o list.o stack.o ADT_Error.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
#GUROBI_COMP = -I/usr/local/lib/gurobi563/include
#GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@
all: $(OBJS)
	$(CC) $(COMP_FLAG) $(OBJS) $(GUROBI_LIB) -o $(EXEC)
main.o: main.c macro.h bool.h ADT_Error.h main_aux.h game.h parcer.h
	$(CC) $(COMP_FLAG) -c $*.c
main_aux.o: main_aux.c main_aux.h bool.h macro.h ADT_Error.h game.h macro.h
	$(CC) $(COMP_FLAG) -c $*.c
ADT_Error.o: ADT_Error.c ADT_Error.h
	$(CC) $(COMP_FLAG) -c $*.c
solve.o: solve.c solve.h game.h ADT_Error.h parcer.h stack.h list.h bool.h macro.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h solve.h stack.h list.h bool.h macro.h ADT_Error.h parcer.h main_aux.h
	$(CC) $(COMP_FLAG) -c $*.c
parcer.o: parcer.c parcer.h main_aux.h game.h list.h bool.h macro.h
	$(CC) $(COMP_FLAG) -c $*.c
list.o: list.c list.h bool.h
	$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.c stack.h list.h bool.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)