#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "macro.h"
#include "bool.h"
#include "ADT_Error.h"
#include "list.h"
#include "game.h"
#include "parcer.h"
#include "main_aux.h"
#include "solve.h"

typedef struct cell_t{
	int r;
	int c;
	bool touch;
} Cell;

/**
 * release func to free item from list
 */
void freeFuncSingleSet(void * _item) {
	free(_item);
}

void freeFuncStepsList(void * _item) {
	list* listInMoveList = (list*)_item;
	list_destroy(listInMoveList);
}

/**
 * erroneous_board - checks if the board contains error values
 *
 * @Input:
 * 	board - the Sudoku board
 *	 N - number of digits in one block
 * @Return ADTErr
 * 	ERR_VALUES - if the board contains erroneous values
 * 	ERR_OK - otherwise
 *
 */
ADTErr erroneous_board(Num** _board, int _N) {
	int i;
	int j;
	for (i=0; i < _N; i++) {
		for (j=0; j < _N; j++) {
			if (_board[i][j].status == ERRONEOUS) {
				return ERR_VALUES;
			}
		}
	}
	return ERR_OK;
}

void free_board(Game* _game) {
	int i=0;
	int N=0;

	if (_game->board != NULL) {
		N = _game->cols *_game->rows;
		for (i = 0; i < N; i++) {
			if (_game->board[i] != NULL) {
				free(_game->board[i]); /*frees all cols*/
				_game->board[i]=NULL;
			}
		}
		free(_game->board); /*frees all rows*/
		_game->board=NULL;
	}
}


void free_board2(Num** board, int N) {
	int i = 0;

	if (board != NULL) {
		for (i = 0; i < N; i++) {
			if (board[i] != NULL) {
				free(board[i]); /*frees all cols*/
			}
		}
		free(board); /*frees all rows*/
	}
}

/**
 * isGameFinish- checks whether the board is filled and all values are legal
 *  @Input:typedef struct cell_t{
		int r;
		int c;
	} Cell;
 * 	_game - the Sudoku game
 * 	@ return bool True/False
 *   **/
bool isGameFinish(Game* _game) {
	int i, j, board_rowsize;
	board_rowsize =_game->cols * _game->rows;
	for (i = 0; i < board_rowsize; i++) {
		for (j= 0; j < board_rowsize; j++) {
			if (_game->board[i][j].num==0) {
				return FALSE;
			}

			else if ( !validate_digit(_game->board, _game->rows, _game->cols,
					i, j, _game->board[i][j].num)) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

Game* game_init() {
	Game* game = (Game*)malloc(1*sizeof(Game));
	game->moveList = (list*)malloc(1*sizeof(list));
	list_new(game->moveList, sizeof(list*), freeFuncStepsList);
	game->board = NULL;
	game->mark_error = TRUE;
	game->mode = INIT;
	game->cols = DEF_COLS;
	game->rows = DEF_ROWS;
	return game;
}

void game_destroy(Game* _game) {
	list_destroy(_game->moveList);
	free(_game->moveList);
	_game->moveList=NULL;
	free_board(_game);
	free(_game);
}

/** copy boards- receive a board and duplicate it.
 * @param old board
 * int _m- size of rows in a block
 * int _n- size of cols in a block
 * @return
 * new board **/

Num** copy_boards(Num** old_board, int _m, int _n) {
	int i;
	Num** new_board;
	new_board = create_empty_board(_m, _n) ;
	for (i = 0; i < _m*_n; i++) {
		memcpy(new_board [i], old_board [i], sizeof(Num)*_m*_n);
	}
	return new_board;
}

void copy_boardsNew(Num** old_board, Num** new_board, int _N) {
	int i, j;
	for (i = 0; i < _N; i++) {
		for (j = 0; j < _N; j++) {
			new_board[i][j].num=old_board[i][j].num;
			new_board[i][j].status=old_board[i][j].status;
		}
	}
	/*printf("copy board\n");*/
}

/**
 * validate-validates the current board using ILP, ensuring it is .
 * if the board is erroneous the program prints an error message and the command is not executed
 * @ Input
 * _game- the sudoko game
 * @ Output
 * ADTErr-is board solvable  **/

ADTErr validate(Game* _game) {
	ADTErr err;
	int _N;
	Num** backupBoard = NULL;
	_N = _game->cols*_game->rows;
	if ( (err = erroneous_board(_game-> board, _N) ) != ERR_OK) {
		return err;
	}
	backupBoard = create_empty_board(_game->rows, _game->cols);
	/** we still need to free the memory allocated for the board.
	 * because the free_board function receive game and not board i'm leaving it for now until we decide
	 * to change the function **/
	err = solve_ilp(_game->board, _game->rows, _game->cols, backupBoard, 1);
	free_board2(backupBoard, _N);
	if (err == ERR_OK){
		printf("board is solvable /n");
	}else{
		printf("board is not sovable");
	}
	return err;
}

/**
 * s - creates a new empty board of the given size.
 * @Input
 * m - number of rows in one block
 * n - number of columns in one blocktypedef struct cell_t{
	int r;
	int c;
} Cell;
 */
Num** create_empty_board(int _m, int _n) {
	int i, j;
	int N = _n * _m;
	Num** board = NULL;
	/*printf("create_empty_board\n");*/
	board = (Num**)malloc(N*sizeof(Num*));
	for (i = 0; i < N; i++) {
		board[i] = (Num*)malloc(N*sizeof(Num));
		for (j = 0; j < N; j++) {
			board[i][j].num = 0;
			board[i][j].status = HIDDEN;
		}
	}
	return board;
}

void setMoveStep(int _row, int _col, int _dig, SingleSet* move_step, Game* _game) {
	move_step->prev_val = _game->board[_row][_col].num;
	move_step->new_val = _dig;
	move_step->col = _col;
	move_step->row = _row;
	move_step->prev_stat = _game->board[_row][_col].status;
}

/**
 * set - sets a cell to the required number.
 * adds the move to the move list,
 * changes relevant cell's status to erroneous and updates the number of hidden cells.
 * when board is full, prints message if it is solved or erroneous. changes mode to INIT if it's solved.
 */
ADTErr set(Game* _game, int _col, int _row, int _dig) {
	SingleSet* move_step;
	list* stepNewList;

	int N = _game->cols * _game->rows;

	_row =_row - 1;
	_col =_col - 1;

	if (_col < 0 || _row < 0 || _dig < 0 || _col >= N || _row >= N || _dig > N) { /*check for range*/
		return INVALID_RANGE;
	}
	if (_game->board[_row][_col].status == FIXED) {
		return CELL_FIX;
	}

	/*set the cell*/
	if (_game->board[_row][_col].num != _dig) { /*if they are the same, board is not changing, and we don't consider it as a move*/

		stepNewList = malloc(sizeof(list));
		list_new(stepNewList, sizeof(SingleSet*), freeFuncSingleSet);
		move_step = malloc(sizeof(SingleSet));
		setMoveStep(_row, _col, _dig, move_step, _game);
		if (_dig == 0) { /*emptying a cell*/
			_game->board[_row][_col].status = HIDDEN;
			_game->board[_row][_col].num = _dig;
			move_step->new_stat = HIDDEN;
		} else if (!validate_digit(_game->board, _game->cols, _game->rows,
				_row, _col, _dig)) {
			_game->board[_row][_col].status = ERRONEOUS;
			_game->board[_row][_col].num = _dig;
			move_step->new_stat = ERRONEOUS;
		} else {
			_game->board[_row][_col].status = SHOWN;
			_game->board[_row][_col].num = _dig;
			move_step->new_stat = SHOWN;
		}

		list_appendAfter(stepNewList, move_step);
		if (_game->moveList->currentElement != NULL
				&& _game->moveList->currentElement->next != NULL) {
			/* delete all node after the current Node*/
			list_deleteAfter(_game->moveList);
		}
		list_appendAfter(_game->moveList, stepNewList);
	}

	if (isGameFinish(_game)) {
		if (erroneous_board(_game->board, N)!= ERR_OK) {
			return BOARD_ERRORS;
		} else {
			mainAux_printGameFinish();
			_game->mode = INIT;
			/* TODO maybe init the game  */
			return PUZZLE_SOLVED;
		}
	}
	return ERR_OK;
}
 
/*************************** help func for generete ********************************/

ADTErr checkIfThereXEmptyCell(Game* _game, int _x, int* _fillcount){
	int i,j,N;
	int count = 0;
	
	N = _game->cols *_game->rows;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if(_game->board[i][j].num==0){
				++count;
			}
		}
	}
	*_fillcount = count;
	return(count>=_x?ERR_OK:NOT_ENOUGH_EMPTY_CELL);
}


void swap(int* randArr, int index1, int index2){
	int temp = randArr[index1];
	randArr[index1] = randArr[index2];
	randArr[index2] = temp;
}

/**
 * Num** backupBoard,
 * int rows,
 * int _cols,
 * Cell* _emptyCellArr - array contain's all indexs of empty cells in
 * _size - _emptyCellArr length
 * int _x - number of empty cell tp fill;
 */
bool backtrackingRand(Num** backupBoard, int _rows, int _cols, Cell* _emptyCellArr,int _size, int _x){
	int* randArr = NULL;
	int value,n, countof_Legalvalues = 0, randomized_index, cell;
	
	/*printf("backtrackingRand\n");*/
	randArr = calloc(sizeof(int),_rows*_cols);
	/*printf("x=%d\n",_x);*/
	if(_x==0){
		if (board_isSolvable(backupBoard,_rows,_cols)){
			/*printf("board_isSolvable\n");*/
			free(randArr);
			return TRUE;
		}
		else{
			/*printf("board not solvable");*/
			free(randArr);
			return FALSE;
		}
	}	
	/*printBoard2(backupBoard,_rows, _cols);*/

	cell = rand()% _size; /* check not the same i as before + touch*/
	while(_emptyCellArr[cell].touch==1){
		cell = rand()% _size;
	}
	/*printf("cell is %d \n",cell);*/
	for (value = 1; value < (_rows*_cols+1); value++)
	{ 
		/*filling the array with legal values.*/
		if (validate_digit(backupBoard, _rows, _cols, _emptyCellArr[cell].r, _emptyCellArr[cell].c, value) == TRUE)
		{
			randArr[countof_Legalvalues] = value;
			countof_Legalvalues++;
			/*printf("legal value  [%d %d] = %d \n",_emptyCellArr[cell].r,_emptyCellArr[cell].c,value);*/
		}
	}

	while (countof_Legalvalues > 1) {
		/*printf("count is bigger than 1\n	");*/
		/*randomly picking and recursively calling the function.
		we have several options and we want to check if one of them is legal solution*/

			randomized_index = rand() % countof_Legalvalues;
			/* as the array with the legal indexes is filled from the first cell it ensures that the size is the
			 * count of legal values and the array is empty from the end until 9 minus count of legal values.
			 * choose randomly an index from legal values  */
			backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].num = randArr[randomized_index];
			backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].status = SHOWN;
			/*printf("value chosen randomly %d \n",(randArr[randomized_index]));*/
			/*printBoard2(backupBoard,_rows, _cols);	*/
			for (n = randomized_index; n < countof_Legalvalues-1; n++){
				swap(randArr, n, n+1);
			}
			countof_Legalvalues--;
			/*now we have one less valid value.*/
			_emptyCellArr[cell].touch = 1;
			if (backtrackingRand(backupBoard, _rows, _cols,_emptyCellArr,_size,--_x) == 1)
			{
				free(randArr);
				return TRUE;
			} 
			else
			{
				backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].num = 0;
				backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].status = SHOWN;
				/*printBoard2(backupBoard,_rows, _cols);*/
				/*delete the value and try again recursivly to check the other options  */
			}
	}
	if (countof_Legalvalues == 1)
	{ 
		/*no need for choosing a value in randomized way as only one is remain.*/
		/*printf("count legal v ==1");*/
		backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].num = randArr[0];
		backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].status = SHOWN;
		/*printBoard2(backupBoard,_rows, _cols);*/
		countof_Legalvalues=0;
		if (backtrackingRand(backupBoard, _rows, _cols,_emptyCellArr,_size,--	_x) == TRUE)
		{
			/* after putting the remained value check if matrix can be solved */
			free(randArr);
			return TRUE;
		}
		else
		{
			backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].num = 0;
			backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].status = HIDDEN;
			/*printBoard2(backupBoard,_rows, _cols);*/
		}
	}
	if (countof_Legalvalues == 0)
	{ /*all values failed for this cell.*/
		/*printf("count legal v ==0");*/
		backupBoard[_emptyCellArr[cell].r][_emptyCellArr[cell].c].status = HIDDEN;
		/*printBoard2(backupBoard,_rows, _cols);*/
		free(randArr);
		return FALSE;
		/* no value remain is a valid one so no  */
	}
	/*printf("board_is Not Solvable\n");*/
	free(randArr);
	return FALSE;
}


/**
 * fill X empty cell, with random valid number
 */
ADTErr fillXCellRand(Num** backupBoard, int _rows, int _cols, int _x, Cell* _emptyCellArr, int _size){
	/* initialize random seed: */
	bool ret;
	/*printf("fillXCellRand\n");*/
	srand (time(NULL));
	ret = backtrackingRand(backupBoard, _rows, _cols, _emptyCellArr, _size, _x);
	return (ret == TRUE ? ERR_OK : BOARD_IS_NOT_SOLVED);
}

void swapCell(Cell* randArr, int index1, int index2){
	int c, r;
	c = randArr[index1].r;
	r = randArr[index1].c;
	randArr[index1].r = randArr[index2].r;
	randArr[index1].c = randArr[index2].c;
	randArr[index2].r = r;
	randArr[index2].c = c;
}

void clearBoardExceptYCell(Game* _game, int _N, int _y){
	int i,j, count, cell, countfill = 0;
	Cell* emptyCellArr = NULL;
	
	/* count the number of cell that are not 0 */
	for(i = 0; i < _N; i++){
		for(j = 0 ; j < _N; j++){
			if(_game->board[i][j].num != 0){
				++countfill;
			}
		}
	}
	/*printf("countfill %d\n", countfill);*/
	
	if(countfill > 0){
		if(_y > countfill){
			/* we will clear the all board */
			_y = countfill;
		}
		count = 0;
		/*printf("clearBoardExceptYCell malloc \n");*/
		emptyCellArr = (Cell*)calloc(sizeof(Cell), countfill);
		/*create arr of cell that are not 0*/
		for(i = 0; i < _N; i++){
			for(j = 0 ; j < _N; j++){
				if(_game->board[i][j].num != 0){
					emptyCellArr[count].c = j;
					emptyCellArr[count].r = i;
					count++;
				}
			}
		}
		srand(time(NULL));
		/*printf("address : %p\n",(void*)emptyCellArr);*/
		while(_y > 0){
			cell = rand() % countfill;
			/*printf("cell %d\n", cell);*/
			_game->board[emptyCellArr[cell].r][emptyCellArr[cell].c].num = 0;
			_game->board[emptyCellArr[cell].r][emptyCellArr[cell].c].status = HIDDEN;
			for (i = cell; i < countfill-1;i++){
				swapCell(emptyCellArr,i,i+1);
			}
			--countfill;
			--_y;
		}
	}

	/*printf("address : %p\n",(void*)emptyCellArr);*/
	free(emptyCellArr);
	list_destroy(_game->moveList);
	list_new(_game->moveList, sizeof(list*), freeFuncStepsList);
}

void getAllEmptyCell(Game* _game, Cell* emptyCellArr){
	int i, j, N;
	int count = 0;
	N = _game->cols *_game->rows;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if(_game->board[i][j].num == 0){
				emptyCellArr[count].c = j;
				emptyCellArr[count].r = i;
				/*printf("empty num %d (%d,%d)\n",count,i,j);*/
				count++;
			}
		}
	}
}

/**
 *
 * get board in edit mode
 * check if there is x empty cell else return ERROR
 * run ILP if no solution found reset the board back to it original state and choose x cell run ILP again , (do it
 * max 1000 iteration else return ERROR)
 * choose y randomly cells and clear the board except the y cell.
 */
ADTErr generate(Game* _game, int _x, int _y) {
	Num** backupBoard = NULL;
	Num** backupBoard2 = NULL;
	int i=0, count=0, N;
	ADTErr err = ERR_OK;
	Cell* emptyCellArr = NULL;
	
	/*printf("generete\n");*/
	
	N = _game->cols*_game->rows;
	if( ERR_OK != checkIfThereXEmptyCell(_game, _x, &count)){
		return NOT_ENOUGH_EMPTY_CELL;
	}
	/*printf("empty cell %d\n", count);*/
	if (erroneous_board(_game->board, N) != ERR_OK) {
		printf("error board %d\n", count);
		return BOARD_ERRORS;
	}
	/*printf("generate malloc \n");*/
	backupBoard = create_empty_board(_game->rows, _game->cols);
	backupBoard2 = create_empty_board(_game->rows, _game->cols);
	/*check if sudoku solvable run ilp*/
	if( solve_ilp(_game->board, _game->rows, _game->cols, backupBoard, 0) != ERR_OK){
		free_board2(backupBoard, _game->rows*_game->cols);
		return BOARD_IS_NOT_SOLVED;
	}
	/*printf("board is solvable\n");*/
	/*printf("generate emptyCellArr malloc\n");*/
	emptyCellArr = (Cell*)calloc(sizeof(Cell), count);
	getAllEmptyCell(_game, emptyCellArr);
	
	while (i < GENERATE_ITE){
		copy_boardsNew(_game->board, backupBoard, N);
		if(fillXCellRand(backupBoard, _game->rows, _game->cols , _x, emptyCellArr, count)==ERR_OK){
			/*printf("board is solvable exit backtracking");*/
			/*printBoard2(backupBoard,_game->rows, _game->cols);*/
			err = solve_ilp(backupBoard, _game->rows, _game->cols, backupBoard2, 0);
			/*printf("after ilp backupBoard2\n");
			printBoard2(backupBoard2,_game->rows, _game->cols);*/
			if(err == ERR_OK){
				/* need to copy values to board */
				copy_boardsNew(backupBoard, _game->board, N);
				/* printf("after copy board game board\n");
				printBoard2(_game->board,_game->rows, _game->cols);
				*/
				clearBoardExceptYCell(_game, N, _y);
				break;
			}
		}
		i++;
		/*printf("i = %d",i);*/
	}
	
	free_board2(backupBoard, _game->rows * _game->cols);
	free_board2(backupBoard2, _game->rows * _game->cols);
	free(emptyCellArr);
	return err;
}

/********************************************* end generate funcs **********************/

ADTErr undo(Game* _game) {
	listNode* listSteps; /* data is list* */
	listNode* sNode; /* data is SingleSet* */
	SingleSet* ss;

	if (_game->moveList!=NULL) {
		listSteps = list_getCurrentElement(_game->moveList);
		if (listSteps != NULL && listSteps->data != NULL) {
			sNode = list_head(listSteps->data);
			while (sNode != NULL) {
				ss = sNode->data;
				_game->board[ss->row][ss->col].num = ss->prev_val;
				_game->board[ss->row][ss->col].status = ss->prev_stat;
				sNode = sNode->next;
			}
			list_undoCurrentElement(_game->moveList);
		} else {
			return CANNOT_UNDO;
		}
	} else {
		return CANNOT_UNDO;
	}
	return ERR_OK;
}

ADTErr redo(Game* _game) {
	listNode* listSteps; /* data is list* */
	listNode* sNode; /* data is SingleSet* */
	SingleSet* ss;

	if (_game->moveList!=NULL) {
		if (list_redoCurrentElement(_game->moveList) == 0) {
			return CANNOT_REDO;
		}
		listSteps = list_getCurrentElement(_game->moveList);
		if (listSteps != NULL && listSteps->data != NULL) {
			sNode = list_head(listSteps->data);
			while (sNode != NULL) {
				ss = sNode->data;
				_game->board[ss->row][ss->col].num = ss->new_val;
				_game->board[ss->row][ss->col].status = ss->new_stat;
				sNode = sNode->next;
			}
		}
	} else {
		return CANNOT_REDO;
	}
	return ERR_OK;
}

/** changed
 * number of solutions- return number of solutions for a cell
 * @param
 * -board
 * int i- row of checked cell
 * int j- col of checked cell
 * int N- size of rows in a block* cols in a block
 * @return
 * number of solutions **/
int cell_Solutions(Num** board, int blockRow, int blockCol, int i, int j) {
	int s, N, count=0;
	N=blockRow*blockCol;
	for (s=1; s<N+1; s++) {
		/**iterate over the values and check which value is legal. if it is legal add 1 to the counter **/
		if (validate_digit(board, blockRow, blockCol, i, j, s)) {
			count++;
		}
	}
	return count;
}

int find_legalValue(Num** board, int blockRow, int blockCol, int i, int j) {
	int s, N;
	N=blockRow*blockCol;
	for (s=1; s<N+1; s++) {
		/**iterate over the values and check which value is legal. if it is legal return the value **/
		if (validate_digit(board, blockRow, blockCol, i, j, s)) {
			return s;
		}
	}
	return -1;
}

/*receives 2 boards and merge the first one with the second*/
void merge_boards(Num** tobeupdated_board, Num** updating_board, int N) {
	int i, j;
	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			if (updating_board[i][j].num!=0) {
				tobeupdated_board[i][j].num=updating_board[i][j].num;
				tobeupdated_board[i][j].status=SHOWN;
			}
		}
	}
}

/**
 * autofill:
 * copy board
 * check on the new board if each cell contains only one leagal value then update thae value on the original board
 *
 * create new list that contain all of the changes and append the list to game move list.
 *
 */

/**changed nadz **/

ADTErr autofill(Game* _game) {
	SingleSet* move_step;
	Num** duplicated_board;
	list* stepNewList;
	int N, i, j, value;

	N=_game->cols*_game->rows;
	/**if board is erroneous return error message **/
	if (erroneous_board(_game->board, N)!= ERR_OK) {
		printf("erroneous");
		return BOARD_ERRORS;
	}
	duplicated_board=create_empty_board(_game->rows, _game->cols);
	/*printBoard2(duplicated_board, _game->rows, _game->cols);*/
	/**create list for all the moves to be done **/
	stepNewList = malloc(sizeof(list));
	list_new(stepNewList, sizeof(SingleSet*), freeFuncSingleSet);
	/** check whether the cell has only one legal value**/
	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			if ((_game->board[i][j].num==0 )&&(cell_Solutions(_game->board,
					_game->rows, _game->cols, i, j)==1))
			/**if the cell has one legal value  add the move to steplist  **/
			{
				value=find_legalValue(_game->board, _game->rows, _game->cols,
						i, j);
				move_step = malloc(sizeof(SingleSet));
				setMoveStep(i, j, value, move_step, _game);
				move_step->new_stat=SHOWN;
				list_appendAfter(stepNewList, move_step);
				/** update the duplicated board to save changes **/
				duplicated_board[i][j].num=value;
			}
		}
	}
	if (_game->moveList->currentElement != NULL
			&& _game->moveList->currentElement->next != NULL) {
		/* delete all node after the current Node*/
		list_deleteAfter(_game->moveList);
	}
	/*insert the stepNewlist to the game movelist */
	list_appendAfter(_game->moveList, stepNewList);
	merge_boards(_game->board, duplicated_board, N);
	free_board2(duplicated_board, N);
	return ERR_OK;
}

ADTErr generalHint(Game* _game, int _userRow, int _userCol, bool _isilp, Num** _backupBoard) {

	ADTErr err;
	int N;
	 N = _game->cols*_game->rows;
	/*errors*/
	
	if (_userCol < 0 || _userRow < 0 || _userCol >= N || _userRow >= N) {
		return INVALID_RANGE;
	}
	if (erroneous_board(_game->board, N)!= ERR_OK) {
		return BOARD_ERRORS;
	}
	if (_game->board[_userCol][_userRow].status == FIXED) {
		return CELL_FIX;
	}
	if (_game->board[_userCol][_userRow].num != 0) {
		return CELL_HAVE_VALUE;
	}
	if (_isilp) {
		err = solve_ilp(_game->board, _game->rows, _game->cols, _backupBoard, 0);
	} else {
		err = solve_lp(_game);
	}
	if (err != ERR_OK) {
		return BOARD_IS_NOT_SOLVED;
	}

	mainAux_printHint(err);
	return ERR_OK;
}

/* ilp */
ADTErr hint(Game* _game, int _userCol, int _userRow) {
	ADTErr err;
	Num** backupBoard = create_empty_board(_game->rows, _game->cols);
	
	err = generalHint(_game, _userRow-1, _userCol-1, TRUE, backupBoard);
	if (err == ERR_OK) {
		printf("hint for cell [%d,%d] is %d ",_userCol, _userRow,backupBoard[_userRow-1][_userCol-1].num);
	}
	/*printBoard2(backupBoard, _game->rows, _game->cols);*/
	free_board2(backupBoard, _game->rows*_game->cols);
	return err;
}
/**
 * writeNumToFile writes the number in a required cell to a given file.
 * @Input
 * 	fp - pointer to file
 * 	board - game's board
 * 	row - row of cell
 * 	col - column of cell
 * 	mode - game's mode
 */
void writeNumToFile(FILE* fp, Num** board, int row, int col, MODE mode) {
	int dig = board[row][col].num;
	if (mode == EDIT) {
		if (dig != 0)
			/* save as fix */
			fprintf(fp, "%d.", dig);
		else
			fprintf(fp, "0");
	} else if (mode == SOLVE) {
		if (board[row][col].status == FIXED) {
			fprintf(fp, "%d.", dig);
		} else if (board[row][col].status == ERRONEOUS) {
			fprintf(fp, "%d*", dig);
		} else {
			fprintf(fp, "%d", dig);
		}
	}
}

/**
 * path /home/eyal/eclipse-workspace/project/temp.txt
 * 
 * /a/home/cc/students/cs/eyalborovsky/workspace/sudoko/temp.txt 
 *
 * 	char* path = realpath(filename, NULL);
 if(path == NULL){
 printf("cannot find file with name[%s]\n", f);
 } else{
 printf("path[%s]\n", path);
 free(path);
 }
 *
 */
ADTErr save(Game* _game, char* _path) {
	FILE* fp;
	int i, j;
	int N = _game->rows * _game->cols;
	ADTErr err;
	if (_game->mode == EDIT) {
		if (erroneous_board(_game->board, N) != ERR_OK) {
			return PUZLLE_ERROR_CANNOT_SAVE;
		}
		if ((err=validate(_game)) != ERR_OK) {
			return err;
		}
	}
	fp = fopen(_path, "w");
	if (!fp) {
		return FILE_CANNOT_OPEN;
	}
	/*saving*/
	fprintf(fp, "%d %d\n", _game->rows, _game->cols);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			writeNumToFile(fp, _game->board, i, j, _game->mode);
			if (j == N-1) {
				fputc('\n', fp);
			} else {
				fputc(' ', fp);
			}
		}
	}
	fclose(fp);
	printf("Saved to: %s\n", _path);
	return ERR_OK;
}

/**
 * solve 	
 * all current work delete copy from editboard_isSolvable
 * initializes the undo/redo list.
 */
ADTErr solve(Game* _game, char* _path) {
	ADTErr res = ERR_OK;
	FILE* fp = fopen(_path, "r");
	if (!fp) {
		return FILE_ERROR;
	}
	game_destroy(_game);
	_game = game_init();
	_game->mode = SOLVE;
	res = parse_file(fp, _game);
	fclose(fp);
	return res;
}

ADTErr reset(Game* _game) {
	if (_game->moveList!=NULL) {
		while (_game->moveList->currentElement != _game->moveList->head) {
			undo(_game);
		}
		undo(_game);
	}
	return ERR_OK;
}

ADTErr change_mark_errors(Game* _game, bool _x) {
	_game->mark_error = _x;
	return ERR_OK;
}

ADTErr edit(Game* _game, char* _parsed_command) {
	FILE* fp= NULL;
	ADTErr res = ERR_OK;

	game_destroy(_game);
	_game = game_init();

	if (_parsed_command == NULL) { /* create empty board 9*9 */
		_game->cols = DEF_COLS;
		_game->rows = DEF_ROWS;
		_game->board = create_empty_board(DEF_ROWS,DEF_COLS);
		res=ERR_OK;
	} else {
		fp = fopen(_parsed_command, "r");
		if (!fp) {
			return FILE_CANNOT_OPEN;
		}

		res = parse_file(fp, _game);
		fclose(fp);
	}
	if (res==ERR_OK) {
		_game->mode = EDIT;
	}
	return res;
}

ADTErr num_of_solutions(Game* _game) {
	int num_solutions, _N;
	ADTErr err;
	Num** duplicated_board;

	_N = _game->cols * _game->rows;
	if ( (err = erroneous_board(_game-> board, _N) ) != ERR_OK) {
		return err;
	} else {
		duplicated_board=create_empty_board(_game->rows, _game->cols);
		copy_boardsNew(_game->board, duplicated_board, _N);

		num_solutions = backtrack_Algo(duplicated_board, _game->rows,
				_game->cols);
		printf(" number of solutions is : %d", num_solutions);
		free_board2(duplicated_board, _N);
	}
	return ERR_OK;
}
/*
 * LP
 */
ADTErr guess_hint(Game* _game, int _userRow, int _userCol) {
	ADTErr err = ERR_OK;
	Num** backupBoard = create_empty_board(_game->rows, _game->cols);
	err = generalHint(_game, _userRow, _userCol, FALSE, backupBoard);
	free_board2(backupBoard, _game->rows*_game->cols);
	return err;
	
}

/*
 * LP
 */
ADTErr guess(Game* _game, float _x) {
	printf("%d%d", (int)_x, _game->cols);
	return ERR_OK;
}

/**
 * print_dashes - prints 4N+m+1 dashes.
 *
 */
void print_dashes(int m, int n) {
	int i;
	for (i=0; i<(4*n*m+m+1); i++) {
		printf("-");
	}
	printf("\n");
}

/**
 * print_num
 * prints the numbers according to their status HIDDEN|SHOWN|FIXED|ERRONEOUS and according to game's mode.
 *
 * @Input
 * 	Num n - number in the cell.
 *  mode - game's mode
 *  mark_errors - indicates whether to mark error
 */
void print_num(Num n, MODE mode, int mark_errors) {

	switch (n.status) {
	case HIDDEN: /*value is 0*/
		printf("    ");
		break;
	case SHOWN:
		printf(" %2d ", n.num);
		break;
	case FIXED:
		if (mode == SOLVE)
			printf(".%2d.", n.num);
		else
			printf(" %2d ", n.num);
		break;
	case ERRONEOUS:
		if (mode == EDIT || mark_errors)
			printf(" %2d*", n.num);
		else
			printf(" %2d ", n.num);
	}
}

ADTErr printBoard(Game* _game) {
	int row;
	int col;
	int N = _game->rows*_game->cols;
	for (row = 0; row < N; row++) {
		if (row % _game->rows == 0) {
			print_dashes(_game->rows, _game->cols);
		}
		for (col = 0; col < N; col++) {
			if (col % _game->cols== 0)
				printf("%s", "|");
			print_num(_game->board[row][col], _game->mode, _game->mark_error);
		}
		printf("%c\n", '|');
	}
	print_dashes(_game->rows, _game->cols);
	return ERR_OK;
}
/** temporart function**/
void printBoard2(Num** board, int Brow, int Bcol) {
	int row;
	int col;
	for (row = 0; row < Brow*Bcol; row++) {
		if (row % Brow == 0) {
			print_dashes(Brow, Bcol);
		}
		for (col = 0; col < Bcol *Brow; col++) {
			if (col % Bcol== 0)
				printf("%s", "|");
			print_num(board[row][col], EDIT, 1);
		}
		printf("%c\n", '|');
	}
	print_dashes(Brow, Bcol);

}

ADTErr exit_game(Game* _game) {
	game_destroy(_game);
	return EXIT;
}

bool validate_row(Num** board, int row, int col, int checked_value, int size) {
	int i;
	for (i = 0; i< size; i++) {
		if (i != col) {
			if (board[row][i].num== checked_value) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

bool validate_col(Num** board, int row, int col, int checked_value, int size) {
	int i;
	for (i = 0; i< size; i++) {
		if (i != row) {
			if (board[i][col].num == checked_value) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

bool validate_block(Num** board, int row, int col, int checked_value,
		int block_rowsize, int block_colsize) {
	int a, b, c, d;
	a = (row / block_rowsize)*block_rowsize;
	b = (col / block_colsize)*block_colsize; /* the same with the col*/
	for (c = 0; c < block_colsize; c++) { /*  iterates over the elements in the block */
		for (d = 0; d < block_rowsize; d++) {
			if ( !((a+c==row) &&(b+d==col))) {
				if (board[a + c][b + d].num == checked_value) {
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/**
 * checks if a placement of a given digit in a given cell is valid,
 * @Input
 * 	 int dig - the checked digit
 * 	 int row - the cell's row
 * 	 int col - the cell's column
 * 	 Game* game

 * @return
 * FALSE - if one of the validations has failed
 * TRUE - if all validations succeeded
 */

bool validate_digit(Num** board, int blockRow, int blockCol, int cell_row, int cell_col, int _dig) {
	int size;
	size=blockCol*blockRow;
	if (_dig == 0)
		return TRUE;
	if (!validate_row(board, cell_row, cell_col, _dig, size))
		return FALSE;
	if (!validate_col(board, cell_row, cell_col, _dig, size))
		return FALSE;
	if (!validate_block(board, cell_row, cell_col, _dig, blockRow, blockCol))
		return FALSE;
	return TRUE;
}

bool board_isSolvable(Num** board, int blockRow, int blockCol) {
	int i, j;
	for (i=0; i<blockRow*blockCol; i++) {
		for (j=0; j<blockRow*blockCol; j++) {
			if (!validate_digit(board, blockRow, blockCol, i, j, board[i][j].num)) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

