#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "macro.h"
#include "bool.h"
#include "list.h"
#include "ADT_Error.h"
#include "game.h"
#include "parcer.h"
#include "solve.h"
#include "stack.h"

#include "gurobi_c.h"

/* updated nadz*/
typedef struct stack_cell {
	int row;
	int col;
	int value;
} StackCell;

void freeFuncSingleStackCell(void * _item) {
	free(_item);
}

void freeAll(GRBmodel* model, GRBenv* env, char* vtype, int* ind, double* val,
		double* sol) {
	if (model!=NULL) {
		GRBfreemodel(model);
	}
	if (env!=NULL) {
		GRBfreeenv(env);
	}

	free(val);
	free(sol);
	free(ind);
	free(vtype);
}

/*
 int findHint(int _userCol, int _userRow, double* sol, int N){
 int i;
 for(i = 0; i< N ;i++){
 if(sol[(_userRow-1)*N*N + (_userCol-1)*N + i]==1.0){
 return i+1;
 }
 }
 return -1;
 }*/

ADTErr solve_ilp( Num** _board, int row, int col, Num** _backupBoard, int _func	) {

	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int i=0, j=0, v, ii, jj, tmp, index;
	int error = 0, boardSize=row*col;
	int *ind = calloc(boardSize, sizeof(int));
	double *val = calloc(boardSize, sizeof(double));
	double *sol = calloc(boardSize*boardSize*boardSize, sizeof(double));
	int optimstatus;
	char *vtype = calloc(boardSize*boardSize*boardSize, sizeof(char));

	/* Create environment */
	if(_func==0){
		/*printf("inside solve ilp run hint \n");*/
	}
	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/*setting the outputFlag to 0 which prevents gurobi from printing to screen*/

	error = GRBsetintparam(env, "OutputFlag", 0);
	if(error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/* Create new model */

	error = GRBnewmodel(env, &model, "sudoku", 0,NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/*define what kind the variables are*/
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				vtype[i*boardSize*boardSize+j*boardSize+v] = GRB_BINARY;
			}
		}
	}

	/* add variables to model */

	error = GRBaddvars(model, boardSize*boardSize*boardSize, 0, NULL, NULL, NULL, NULL, NULL, NULL, vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/* Each cell gets a single value */

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				ind[v] = i*boardSize*boardSize + j*boardSize + v;
				val[v] = 1.0;
			}
			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				freeAll(model, env, vtype, ind, val, sol);
				return ILP_ERROR;
			}
		}
	}

	/* Each value appears once in each row */

	for (v = 0; v < boardSize; v++) {
		for (j = 0; j < boardSize; j++) {
			for (i = 0; i < boardSize; i++) {
				ind[i] = i*boardSize*boardSize + j*boardSize + v;
				val[i] = 1.0;
			}
			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				freeAll(model, env, vtype, ind, val, sol);
				return ILP_ERROR;
			}
		}
	}

	/* Each value appears once in each column */

	for (v = 0; v < boardSize; v++) {
		for (i = 0; i < boardSize; i++) {
			for (j = 0; j < boardSize; j++) {
				ind[j] = i*boardSize*boardSize + j*boardSize + v;
				val[j] = 1.0;
			}
			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				freeAll(model, env, vtype, ind, val, sol);
				return ILP_ERROR;
			}
		}
	}

	/* Each value appears once in each block */

	for (v = 0; v < boardSize; v++) {
		for (ii = 0; ii < col; ii++) {
			for (jj = 0; jj < row; jj++) {
				index = 0;
				for (i = ii*row; i < (ii+1)*row; i++) {
					for (j = jj*col; j < (jj+1)*col; j++) {
						ind[index] = i*boardSize*boardSize + j*boardSize + v;
						val[index] = 1.0;
						index++;
					}
				}
				error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					freeAll(model, env, vtype, ind, val, sol);
					return ILP_ERROR;
				}
			}
		}
	}

	/*each non-empty cell [i][j] in state.board means the variable x[i,j,v] equals 1.0*/
	for(i=0; i<boardSize; i++) {
		for(j=0; j<boardSize; j++) {
			if(_board[i][j].num != 0) {
				tmp=_board[i][j].num;
				ind[0] = i*boardSize*boardSize + j*boardSize + tmp -1;
				val[0] = 1.0;
				error = GRBaddconstr(model, 1, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					freeAll(model, env, vtype, ind, val, sol);
					return ILP_ERROR;
				}
			}
		}
	}

	/* Optimize model - need to call this before calculation */
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/* Write model to 'mip1.lp' - this is not necessary but very helpful */
	error = GRBwrite(model, "mip1.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	}

	/* Get solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_ERROR;
	}

	/* get the solution - the assignment to each variable */
	/* Dim*Dim*Dim number of variables, the size of "sol" should match */

	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, boardSize*boardSize*boardSize, sol);
	if (error) {
		/*printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));*/
		freeAll(model, env, vtype, ind, val, sol);
		/*return ILP_ERROR;*/
		return ILP_NO_SOLUTION;
	}

	/* solution found */
	if (optimstatus == GRB_OPTIMAL) {
		for(i=0; i<boardSize; i++) {
			for(j=0; j<boardSize; j++) {
				for(v=0; v<boardSize; v++) {
					if(sol[i*boardSize*boardSize+j*boardSize+v]==1.0) {
						_backupBoard[i][j].num=v+1;
						_backupBoard[i][j].status=SHOWN;
					}
				}
			}
		}
		freeAll(model, env, vtype, ind, val, sol);
		return ERR_OK;
	}
	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD) {
		freeAll(model, env, vtype, ind, val, sol);
		return ILP_NO_SOLUTION;
	}

	/* error or calculation stopped */
	else {
		printf("Optimization was stopped early\n");
		freeAll(model, env, vtype, ind, val, sol);
	}
	freeAll(model, env, vtype, ind, val, sol);
	return ILP_ERROR;
}
/* ************************** end of solve ilp ************************************/
	
	
ADTErr solve_lp(Game* _game) {
	printf("%d", _game->cols);
	return ERR_OK;
}

/** changed nadz**/
int findnext_legalvalue(Num** board, int blockRow, int blockCol,
		StackCell* cell, int currentValue) {
	int size, i;
	size= blockRow* blockRow;
	if (cell->row!=-1) {

		for (i=currentValue; i<size+1; i++) {
			if (validate_digit(board, blockRow, blockCol, cell->row,
							cell->col, i)) {
				return i;
			}
		}
	}
	return -1;
}

/**changed nadz**/
void find_nextEmptyCell(Num** board, int board_size,
		StackCell* board_cell) {
	int i, j;

	for (i=0; i<board_size; i++) {
		for (j=0; j<board_size; j++) {
			if ( (board[i][j].num==0)&&(board[i][j].status!=FIXED)) {
				board_cell->row=i;
				board_cell->col=j;
				board_cell->value=-1;
				return;
			}
		}
	}
	board_cell->row=-1;
	board_cell->col=-1;
	board_cell->value=0;
}

bool board_isFinish(Num** board, int blockRow, int blockCol) {
	int i, j;
	for (i=0; i<blockRow*blockCol; i++) {
		for (j=0; j<blockRow*blockCol; j++) {
			if (!validate_digit(board, blockRow, blockCol, i, j, board[i][j].num)|| (board[i][j].num==0)) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

/**changed nadz **/
int backtrack_Algo(Num** temp_board, int block_row, int block_col) {

	int count, old_value, size, row, col;
	StackCell* next_cell;
	StackCell* dummy_cell;
	StackCell* item2;
	stack* mystack;

	count = 0;
	mystack = malloc(1 * sizeof(stack));
	size = block_row * block_col;
	stack_new(mystack, sizeof(StackCell), freeFuncSingleStackCell);
	next_cell = malloc(sizeof(StackCell));
	item2 = malloc(sizeof(StackCell));

	/** find next empty cell. if no cell is empty we get value of -1 in row and in column. **/
	find_nextEmptyCell(temp_board, size, next_cell);
	next_cell->value=findnext_legalvalue(temp_board, block_row,
			block_col, next_cell, 1);
	if ((next_cell->col != -1) && (next_cell->value != -1)) {

		stack_push(mystack, next_cell);

		row = next_cell->row;
		col = next_cell->col;
		temp_board[row][col].num = next_cell->value;
		temp_board[row][col].status=SHOWN;
	}
	while (stack_size(mystack) != 0) {
		/**freeFuncSingleStackCell(next_cell);**/
		next_cell = malloc(sizeof(StackCell));
		find_nextEmptyCell(temp_board, size, next_cell);
		row = next_cell->row;
		col = next_cell->col;

		next_cell->value = findnext_legalvalue(temp_board, block_row,
				block_col, next_cell, 1);

		if ((next_cell->col != -1)&&(next_cell->value != -1)) {
			stack_push(mystack, next_cell);

			row = next_cell->row;
			col = next_cell->col;
			temp_board[row][col].num = next_cell->value;
			temp_board[row][col].status=SHOWN;
		} else {

			if (board_isFinish(temp_board, block_row, block_col)) {
				count++;
				/**	printBoard2(temp_board, block_row, block_col);**/
			}
			dummy_cell= (StackCell*)stack_peek(mystack);
			row =dummy_cell->row;
			col = dummy_cell->col;
			old_value= dummy_cell->value;
			dummy_cell->value = findnext_legalvalue(temp_board,block_row, block_col, dummy_cell, old_value+1);
			while (dummy_cell->value==-1) {
				temp_board[row][col].num = 0;
				if (stack_size(mystack) > 0) {
					stack_pop(mystack);
					if (stack_size(mystack) > 0) {
						dummy_cell= (StackCell*)stack_peek(mystack);
						dummy_cell->value= findnext_legalvalue(
								temp_board, block_row, block_col,
								dummy_cell, dummy_cell->value+1);
						row=dummy_cell->row;
						col=dummy_cell->col;
					}
				} else {
					dummy_cell->row=row;
					dummy_cell->col=col;
					break;
				}
			}
			if (dummy_cell->value!=-1) {
				row = dummy_cell->row;
				col = dummy_cell->col;
				temp_board[row][col].num = dummy_cell->value;
			}
		}

	}
	stack_destroy(mystack);
	free(dummy_cell);
	free(next_cell);
	free(item2);
	return count;
}

