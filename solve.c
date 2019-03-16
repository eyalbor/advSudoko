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
/** this functions are temporary see what I wrote in the h file **/ 
ADTErr solve_ilpDuplicated(Game* _game, Num** temp_board){
	return ERR_OK;
}

ADTErr solve_lpDuplicated(Game* _game, Num** temp_board){
	return ERR_OK;
}

/** changed nadz**/
int findnext_legalvalue(Num** board, int blockRow, int blockCol,StackCell* cell, int currentValue){
	int size, i;
	size= blockRow* blockRow;
	if(cell->row!=-1)
	{

		for(i=currentValue; i<size+1; i++)
		{
			if (validate_digit(board,blockRow,blockCol,cell->row, cell->col,i))
			{
				return i;
			}
		}
	}
	return -1;
}

/**changed nadz**/

void find_nextEmptyCell(Num** board,int board_size, StackCell* board_cell){
	int i, j;

	for(i=0; i<board_size;i++)
	{
		for(j=0;j<board_size;j++)
		{
			if( (board[i][j].num==0)&&(board[i][j].status!=FIXED))
			{
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

/**changed nadz **/

int backtrack_Algo(Num** temp_board, int block_row, int block_col)
{

	int count = 0,old_value,size, row, col;
	StackCell* next_cell;
	StackCell* dummy_cell;
	StackCell* item2;

	stack* mystack = malloc (1 * sizeof(stack));
	size = block_row * block_col;
	stack_new ( mystack , sizeof (StackCell) , freeFuncSingleStackCell );
	next_cell = malloc ( sizeof ( StackCell ) );
	item2 = malloc ( sizeof ( StackCell ) );

	/** find next empty cell. if no cell is empty we get value of -1 in row and in column. **/
	 find_nextEmptyCell (temp_board,size,next_cell);
	 next_cell->value=findnext_legalvalue(temp_board, block_row, block_col,next_cell, 1);
	if ((next_cell->col != -1) && (next_cell->value != -1))
	{

		stack_push (mystack, next_cell);

		row = next_cell->row;
		col = next_cell->col ;
		temp_board[row][col].num = next_cell->value;
		temp_board[row][col].status=SHOWN;
	}
	while (stack_size (mystack) != 0)
	{
		/**freeFuncSingleStackCell(next_cell);**/
		next_cell = malloc ( sizeof ( StackCell ) );
		find_nextEmptyCell (temp_board,size,next_cell);
		row = next_cell->row;
		col = next_cell->col ;

		next_cell->value = findnext_legalvalue( temp_board, block_row, block_col,next_cell, 1);

		if ((next_cell->col != -1)&&(next_cell->value != -1))
				{

					stack_push (mystack,next_cell);

					row = next_cell->row;
					col = next_cell->col ;
					temp_board[row][col].num = next_cell->value;
					temp_board[row][col].status=SHOWN;

				}
		else
		{

			if (board_isSolvable(temp_board, block_row,block_col))
			{
				count++;
			/**	printBoard2(temp_board, block_row, block_col);**/
			}
			dummy_cell= (StackCell*)stack_peek(mystack);
			row =dummy_cell->row;
			col = dummy_cell->col ;
			old_value= dummy_cell->value;
			dummy_cell->value = findnext_legalvalue ( temp_board , block_row , block_col ,dummy_cell , old_value+1);
			while(dummy_cell->value==-1)
			{
				temp_board[row][col].num = 0;
				if(stack_size(mystack) > 0)
				{
				stack_pop (mystack, item2);
				if(stack_size(mystack) > 0)
					{
					dummy_cell= (StackCell*)stack_peek(mystack);
					dummy_cell->value= findnext_legalvalue ( temp_board , block_row , block_col ,	dummy_cell ,  dummy_cell->value+1);
					row=dummy_cell->row;
					col=dummy_cell->col;
					}
				}
				else
				{
					dummy_cell->row=row;
					dummy_cell->col=col;
					break;
				}
			}
			if(dummy_cell->value!=-1)
			{
				row = dummy_cell->row;
				col = dummy_cell->col ;
				temp_board[row][col].num = dummy_cell->value;


			}
		}

	}
	stack_destroy(mystack);
	freeFuncSingleStackCell(dummy_cell);
	freeFuncSingleStackCell(next_cell);
	freeFuncSingleStackCell(item2);
	return count;
}

