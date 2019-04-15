/**
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
/**
 * obj is Coefficients maximize func
 * val constraint Coefficients
 * vtype option other options: GRB_BINARY, 	GRB_INTEGER, GRB_CONTINUOUS 
 
void initCoefficients(double* obj, double* val, char* vtype, int array3Size) {
	int i;
	for (i = 0; i < array3Size; i++) {
		obj[i] = 1.0;
		val[i] = 1.0;
		vtype[i] = GRB_BINARY;
	}
}

void allocateArrs(double **obj, char **vtype, int **ind, double **val,
		double **sol, int n) {
	int array3Size = n*n*n;
	*obj = (double *) malloc(array3Size* sizeof(double));
	*vtype = (char *) malloc(array3Size* sizeof(char));
	*ind = (int *) malloc(n * sizeof(int));
	*val = (double *) malloc(array3Size* sizeof(double));
	*sol = (double *) malloc(array3Size * sizeof(double));
}

void freeAll(GRBmodel* model, GRBenv* env, double* obj, char* vtype, int* ind,
		double* val, double* sol) {
	if (model!=NULL) {
		GRBfreemodel(model);
	}
	if (env!=NULL) {
		GRBfreeenv(env);
	}

	free(obj);
	free(val);
	free(sol);
	free(ind);
	free(vtype);
}

/**
 * N = arraySingleRowSize;
 
int addRowConstrains(GRBmodel **model, int* ind, double* val, int N) {
	int i, j, k, error = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				ind[k] = k*N*N + j*N + i;
				val[k] = 1.0;
			}
			error = GRBaddconstr(*model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				return 0;
			}
		}
	}
	return 1;
}

int addCellConstrains(GRBmodel **model, int* ind, double* val, int N) {
	int i, j, k, error;
	error = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
						ind[k] = i*N*N + j*N + k;
						val[k] = 1.0;
					}
					error = GRBaddconstr(*model, N, ind, val, GRB_EQUAL, 1.0, 
					NULL);
					/*printf("error = %d", error);	
					if (error) {
						return 0;
					}
				}
			}
			return 1;
		}

		int addColConstrains(GRBmodel **model, int* ind, double* val, int N) {
			int i, j, k, error = 0;
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					for (k = 0; k < N; k++) {
						ind[k] = j*N*N + k*N + i;
						val[k] = 1.0;
					}
					error = GRBaddconstr(*model, N, ind, val, GRB_EQUAL, 1.0, 
					NULL);
					if (error) {
						return 0;
					}
				}
			}
			return 1;
		}

		int addBlockConstrains(GRBmodel **model, int* ind, double* val,
				int col, int row) {

			int i, j, k, current_row, current_col, count, error;
			for (i = 0; i < row*col; i++) {
				for (current_col = 0; current_col < col; current_col++) {
					for (current_row = 0; current_row < row; current_row++) {
						for (j = current_col*row; j < (current_col+1)*row; j++) {
							for (k = current_row*col; k < (current_row+1)*col; k++) {
								ind[count]=j*col*col*row*row+k*row*col+i;
								val[count]=1.0;
								count++;
							}
						}
						error = GRBaddconstr(*model, row*col, ind, val,
								GRB_EQUAL, 1.0, NULL);
						if (error) {
							return 0;
						}
					}
				}
			}
			return 1;
		}


		int addFilledCellsConstrains(GRBmodel **model, int* ind, double* val,Num** board,  int N) {
			int i, j, error = 0;
			for (i = 0; i < N; i++) { /*for each row
				for (j = 0; j < N; j++) { /*for each column
					if (board[j][i].num != 0) {
						ind[0] = (N * i + j) * N + board[i][j].num - 1;
						printf("int = %d\n", ind[0] );
						val[0]= 1.0;
						error = GRBaddconstr(*model, 1, ind, val, GRB_EQUAL,1.0, NULL);
						if (error) {
							return 0;
						}
					}
				}
			}
			return 1;
		}

		int findHint(int _userCol, int _userRow, double* sol, int N){
			int i;
			for(i = 0; i< N ;i++){
				if(sol[_userRow*N*N + _userCol*N + i]==1.0){
					return i+1;
				}
			}
			return -1;
		}
		
		/* 
		 * int func : 0 - hint, 1 - validate , 2 - generate
		 * 
		ADTErr solve_ilp(Game* _game, int _userRow, int _userCol, int* _result, int _func) {

			GRBenv *env= NULL;
			GRBmodel *model= NULL;
			int error = 0;
			int optimstatus;
			double *val, *obj, *sol;
			int *ind;
			char* vtype;
			int rowConstrains = 1, colConstrains = 1, blockConstrains = 1,
					cellConstrains = 1, filledCellsConstrain = 1;
			int arraySingleRowSize = (_game->cols)*_game->rows;
			int array3Size = (_game->cols*_game->rows)*(_game->cols*_game->rows)
					*(_game->cols*_game->rows);

			printf("solve_ilp\n");
			/* Create environment - log file is mip1.log 
			error = GRBloadenv(&env, "mip1.log");
			if (error) {
				printf("ERROR %d GRBloadenv(): %s\n", error,
						GRBgeterrormsg(env));
				return ILP_ERROR;
			}

			error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
			if (error) {
				GRBfreeenv(env);
				printf("ERROR %d GRBsetintattr(): %s\n", error,
						GRBgeterrormsg(env));
				return ILP_ERROR;
			}

			/* Create an empty model named "mip1" 
			error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, 
			NULL);
			if (error) {
				GRBfreeenv(env);
				printf("ERROR %d GRBnewmodel(): %s\n", error,
						GRBgeterrormsg(env));
				return ILP_ERROR;
			}
			/*allocate arrays
			allocateArrs(&obj, &vtype, &ind, &val, &sol, arraySingleRowSize);
			/* Add variables */
			/* coefficients
			initCoefficients(obj, val, vtype, array3Size);
			/* add variables to model 
			error = GRBaddvars(model, array3Size, 0, NULL, NULL, NULL, NULL, 
			NULL, NULL, vtype, NULL);
			if (error) {
				printf("ERROR %d GRBaddvars(): %s\n", error,
						GRBgeterrormsg(env));
				freeAll(model, env, obj, vtype, ind, val, sol);
				return ILP_ERROR;
			}
			/* Change objective sense to maximization 
			error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
			if (error) {
				printf("ERROR %d GRBsetintattr(): %s\n", error,
						GRBgeterrormsg(env));
				freeAll(model, env, obj, vtype, ind, val, sol);
				return ILP_ERROR;
			}
			/* update the model - to integrate new variables 

			error = GRBupdatemodel(model);
			if (error) {
				printf("ERROR %d GRBupdatemodel(): %s\n", error,
						GRBgeterrormsg(env));
				freeAll(model, env, obj, vtype, ind, val, sol);
				return ILP_ERROR;
			}

			/* constraint
			cellConstrains = addCellConstrains(&model, ind, val,
					arraySingleRowSize); /*for each cell 
			rowConstrains = addRowConstrains(&model, ind, val,
					arraySingleRowSize);/*for each row
			colConstrains = addColConstrains(&model, ind, val,
					arraySingleRowSize); /*for each column
			blockConstrains = addBlockConstrains(&model, ind, val, _game->cols,
					_game->rows); /*for each block
			filledCellsConstrain = addFilledCellsConstrains(&model, ind, val,
					_game->board, arraySingleRowSize); /*for each filled cell*/
			/*check all constrains valid
		/*	printf("%d %d %d %d %d",cellConstrains,rowConstrains,colConstrains, blockConstrains,filledCellsConstrain);
			if ((cellConstrains && rowConstrains && colConstrains
					&& blockConstrains && filledCellsConstrain) != 1) {
				printf("ERROR %d GRBConstrains(): %s\n", error,
						GRBgeterrormsg(env));
				return ILP_ERROR;
			}

			printf("vvvv0");
			/* Optimize model - need to call this before calculation 
			error = GRBoptimize(model);
			if (error) {
				printf("ERROR %d GRBoptimize(): %s\n", error,
						GRBgeterrormsg(env));
				freeAll(model, env, obj, vtype, ind, val, sol);
				return ILP_ERROR;
			}
			printf("vvvv1");
			/* Get solution information 
			error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
			if (error) {
				printf("ERROR %d GRBgetintattr(): %s\n", error,
						GRBgeterrormsg(env));
				return ILP_ERROR;
			}

			/* get the solution - the assignment to each variable 
			printf("vvvv2");
			error = GRBgetdblattrarray(model, "LB", 0, array3Size,
					sol);
			if (error) {
				printf("ERROR %d GRBgetdblattrarray(): %s\n", error,
						GRBgeterrormsg(env));
				/*freeAll(model, env, obj, vtype, ind, val, sol);
				return ILP_ERROR;
			}
			printf("vvvv3");
			/* solution found 
			if (optimstatus == GRB_OPTIMAL) {
				if(_func == 0){
					*_result = findHint(_userCol, _userRow, sol, arraySingleRowSize);
				}else if(_func == 1){
					
				}else{
					
				}
				return ERR_OK;
			}
			/*else no solution found 
			freeAll(model, env, obj, vtype, ind, val, sol);
			return ILP_NO_SOLUTION;
		}

		ADTErr solve_lp(Game* _game) {
			printf("%d", _game->cols);
			return ERR_OK;
		}
**/