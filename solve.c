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


typedef struct stack_cell{
	int row;
	int col;
	int value;
} StackCell;

void freeFuncSingleStuchCell(void * _item){
	free(_item);
}

ADTErr solve_ilp(Game* _game){
	return ERR_OK;
}

ADTErr solve_lp(Game* _game){
	return ERR_OK;
}

int findnext_legalvalue(Num** board, int blockRow, int blockCol,StackCell* cell, int currentValue){
	int size;
	size= blockRow* blockRow*blockCol*blockCol;
	for(; currentValue<size; currentValue++)
	{
		if (validate_digit(board,blockRow,blockCol,cell->row, cell->col,currentValue))
		{
			return currentValue;
		}
	}
	return -1;
}


StackCell* find_nextEmptyCell(Num** board,int board_size){
	int i, j;
	StackCell* board_cell;

	for(i=0; i<board_size;i++)
	{
		for(j=0;j<board_size;j++)
		{
			if( board[i][j].num==0)
			{
				board_cell->row=i;
				board_cell->col=j;
				board_cell->value=0;
				return board_cell;
			}
		}
	}
	board_cell->row=-1;
	board_cell->col=-1;
	board_cell->value=0;
	return board_cell;
}

/** this function finds number of possible solutions using a stack **/
int backtrack_Algo(Num** current_board, int block_row, int block_col)
{
	int count = 0,old_value,size, row, col;
	StackCell* next_cell;
	StackCell dummy_cell;

	stack* mystack = malloc(1*sizeof(stack));
	/** create the stack**/
	size = block_row*block_col;
	stack_new(mystack,sizeof(StackCell),freeFuncSingleStuchCell);
	next_cell = malloc(sizeof(StackCell));
	/** find next empty cell. if no cell is empty we get value of -1 in row and in column. **/
	next_cell = find_nextEmptyCell(current_board,size);
	next_cell->value = findnext_legalvalue( current_board, block_row, block_col,next_cell, 1);
	if ((next_cell->col != -1) && (next_cell->value != -1))
	{
		stack_push(mystack, next_cell);
		row = next_cell->row - 1;
		col = next_cell->col -1;
		current_board[row][col].num = next_cell->value;
	}
	while(stack_size(mystack)!=0)
	{
		next_cell= find_nextEmptyCell(current_board,size);
		if(next_cell->col!=-1)
		{
			stack_push(mystack,next_cell);
		}
		else
		{
			if(board_isSolvable(current_board, block_row,block_col))
			{
				count++;
			}
			stack_pop(mystack,next_cell);
			old_value=next_cell->value;
			next_cell->value=findnext_legalvalue( current_board, block_row, block_col,next_cell, old_value+1);
			if(next_cell->value==-1)
			{
				stack_pop(mystack, &dummy_cell);
			}else
			{
				stack_push(mystack,next_cell);
			}
		}
	}
	return count;
}

