#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "macro.h"
#include "bool.h"
#include "list.h"
#include "ADT_Error.h"
#include "game.h"
#include "parcer.h"
#include "main_aux.h"
#include "solve.h"

/**
 * release func to free item from list
 */
void freeFuncSingleSet(void * _item){
	free(_item);
}

void freeFuncStepsList(void * _item){
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
	int i; int j;
	for(i=0; i < _N; i++){
		for(j=0; j < _N; j++){
			if(_board[i][j].status == ERRONEOUS){
				return ERR_VALUES;
			}
		}
	}
	return ERR_OK;
}

void free_board(Game* _game){
	 int i = 0;
	 int N = _game->cols *_game->rows;

	 if(_game->board != NULL)
	 {
		 for (i = 0; i < N; i++) {
			 if(_game->board[i] != NULL){
				 free(_game->board[i]); /*frees all cols*/
			 }
		}
		free(_game->board); /*frees all rows*/
	 }
}


/**isGameFinish- checks whether the board is filled and all values are legal
 *  @Input:
* 	_game - the Sudoku game
* 	@ return bool True/False
 *   **/
bool isGameFinish(Game* _game){
	int i , j , board_rowsize;
	board_rowsize =_game->cols * _game->rows;
	for ( i = 0 ; i < board_rowsize ; i++)
	{
		for ( j= 0 ; j < board_rowsize ; j++)
		{
			if(_game->board[i][j].num==0)
			{
				return FALSE;
			}

			else if ( ! validate_digit (_game->board, _game->rows, _game->cols , i ,  j , _game->board[i][j].num))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


Game* game_init(){
	Game* game = (Game*)malloc(sizeof(game));
	game->moveList = malloc(sizeof(list));
	list_new(game->moveList,sizeof(list*),freeFuncStepsList);
	game->board = NULL;
	game->mark_error = TRUE;
	game->mode = INIT;
	game->cols = DEF_COLS;
	game->rows = DEF_ROWS;
	return game;
}

void game_destroy(Game* _game){
	list_destroy(_game->moveList);
	free_board(_game);
	free(_game);
}

/** copy boards- receive a board and duplicate it.
 * @param
 * -old board
 * int _m- size of rows in a block
 * int _n- size of cols in a block
 * @return
 * new board **/

Num** copy_boards (Num** old_board , int _m, int _n){
	int i;
	Num** new_board;
	new_board = create_empty_board ( _m ,  _n) ;
	for ( i = 0 ;  i < _m*_n ; i++ )
	{
		memcpy ( new_board [i] , old_board [i] , _m*_n);
	}
	return new_board;
}




/**validate-validates the current board using ILP, ensuring it is solvable.
 * if the board is erroneous the program prints an error message and the command is not executed
 * @ Input
 * _game- the sudoko game
 * @ Output
 * ADTErr-is board solvable  **/

ADTErr validate (Game* _game){
	ADTErr err;
	int _N;
	Num** current_board;
	_N=_game->cols*_game->rows;
	if ( ( err = erroneous_board ( _game-> board , _N ) ) != ERR_OK )
	{
		return err;
	}
	else{
		 current_board=copy_boards ( _game->board ,_game->rows , _game->cols);
		err= solve_ilpDuplicated( _game , current_board);
	}
	/** we still need to free the memory allocated for the board.
	 * because the free_board function receive game and not board i'm leaving it for now until we decide
	 * to change the function **/
	return err;
}


/**
 * create_empty_board - creates a new empty board of the given size.
 * @Input
 * m - number of rows in one block
 * n - number of columns in one block
 */
Num** create_empty_board(int _m, int _n){
	int N = _n * _m;
	int i; int j;
	Num** board = calloc(N,sizeof(Num*));
	for (i = 0; i < N; i++) {
		board[i] = calloc(N,sizeof(Num*));
		for (j = 0; j < N; j++) {
			board[i][j].num = 0;
			board[i][j].status = HIDDEN;
			//board[i][j].alt_num=0;
		}
	}
	return board;
}

void setMoveStep(int _row, int _col, int _dig, SingleSet* move_step,
		Game* _game) {
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
ADTErr set ( Game* _game, int _col, int _row, int _dig){
	SingleSet* move_step;
	list* stepNewList;

	int N = _game->cols * _game->rows;

	_row =_row - 1;
	_col =_col - 1;

	if(_col < 0 || _row < 0 || _dig < 0 || _col >= N || _row >= N || _dig > N){ /*check for range*/
		return INVALID_RANGE;
	}
	if (_game->board[_row][_col].status == FIXED) {
		return CELL_FIX;
	}

	/*set the cell*/
	if (_game->board[_row][_col].num != _dig) { /*if they are the same, board is not changing, and we don't consider it as a move*/

		stepNewList = malloc(sizeof(list));
		list_new(stepNewList, sizeof(SingleSet*) ,freeFuncSingleSet);
		move_step = malloc(sizeof(SingleSet));
		setMoveStep(_row, _col, _dig, move_step, _game);
		if (_dig == 0) { /*emptying a cell*/
			_game->board[_row][_col].status = HIDDEN;
			_game->board[_row][_col].num = _dig;
			move_step->new_stat = HIDDEN;
		}
		else if (!validate_digit(_game->board,_game->cols,_game->rows,_row,_col,_dig)){
			_game->board[_row][_col].status = ERRONEOUS;
			move_step->new_stat = ERRONEOUS;
		}
		else{
			_game->board[_row][_col].status = SHOWN;
			_game->board[_row][_col].num = _dig;
			move_step->new_stat = SHOWN;
		}

		list_appendAfter(stepNewList,move_step);
		if(_game->moveList->currentElement != NULL && _game->moveList->currentElement->next != NULL){
			/* delete all node after the current Node*/
			list_deleteAfter(_game->moveList);
		}
		list_appendAfter(_game->moveList,stepNewList);
	}


	if(isGameFinish(_game)){
		if (erroneous_board(_game->board,N)!= ERR_OK) {
			return BOARD_ERRORS;
		}
		else{
			mainAux_printGameFinish();
			_game->mode = INIT;
			/* TODO maybe init the game  */
			return PUZZLE_SOLVED;
		}
	}
	return ERR_OK;
}

/**
 * doing 1000 iteration else return ERROR
 */
ADTErr generate (Game* _game, int _x, int _y){
	return ERR_OK;
}

ADTErr undo (Game* _game){
	listNode* listSteps; /* data is list* */
	listNode* sNode; /* data is SingleSet* */
	SingleSet* ss;

	if(_game->moveList!=NULL){
		listSteps = list_getCurrentElement(_game->moveList);
		if(listSteps != NULL && listSteps->data != NULL){
			sNode = list_head(listSteps->data);
			while(sNode != NULL){
				ss = sNode->data;
				_game->board[ss->row][ss->col].num = ss->prev_val;
				_game->board[ss->row][ss->col].status = ss->prev_stat;
				sNode = sNode->next;
			}
		}
		list_undoCurrentElement(_game->moveList);
	}
	return ERR_OK;
}

ADTErr redo (Game* _game){
	listNode* listSteps; /* data is list* */
	listNode* sNode; /* data is SingleSet* */
	SingleSet* ss;

	if(_game->moveList!=NULL){
		list_redoCurrentElement(_game->moveList);
		listSteps = list_getCurrentElement(_game->moveList);
		if(listSteps != NULL && listSteps->data != NULL){
			sNode = list_head(listSteps->data);
			while(sNode != NULL){
				ss = sNode->data;
				_game->board[ss->row][ss->col].num = ss->new_val;
				_game->board[ss->row][ss->col].status = ss->new_stat;
				sNode = sNode->next;
			}
		}
	}
	return ERR_OK;
}

ADTErr autofill (Game* _game){
	return ERR_OK;
}

ADTErr generalHint (Game* _game, int _userRow, int _userCol, bool isilp){

	int res;
	int N = _game->cols*_game->rows;
	/*errors*/
	if (_userCol < 0 || _userRow < 0 || _userCol >= N || _userRow >= N){
		return INVALID_RANGE;
	}
	if (erroneous_board(_game->board,N)!= ERR_OK) {
		return BOARD_ERRORS;
	}
	if (_game->board[_userRow][_userCol].status == FIXED) {
		return CELL_FIX;
	}
	if (_game->board[_userRow][_userCol].num != 0) {
		return CELL_HAVE_VALUE;
	}
	if(isilp){
		res = solve_ilp(_game);
	}
	else{
		res = solve_lp(_game);
	}
	if (res != ERR_OK) {
		return BOARD_IS_NOT_SOLVED;
	}

	mainAux_printHint(res);
	return ERR_OK;
}

/* ilp */
ADTErr hint (Game* _game, int _userRow, int _userCol){
	return generalHint(_game,_userRow,_userCol,TRUE);
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
void writeNumToFile (FILE* fp, Num** board, int row, int col, MODE mode) {
	int dig = board[row][col].num;
	if (mode == EDIT) {
		if ( dig != 0)
			/* save as fix */
			fprintf(fp,"%d.",dig);
		else
			fprintf(fp, "0");
	}
	else if (mode == SOLVE) {
		if (board[row][col].status == FIXED){
			fprintf(fp,"%d.",dig);
		}
		else{
			fprintf(fp,"%d",dig);
		}
	}
}

ADTErr save (Game* _game, char* _path){
	FILE* fp;
	int i, j;
	int N = _game->rows * _game->cols;
	ADTErr err;
	if (_game->mode == EDIT) {
		if(erroneous_board(_game->board,N) != ERR_OK){
			return PUZLLE_ERROR_CANNOT_SAVE;
		}
		if((err=validate(_game)) != ERR_OK){
			return err;
		}
	}
	fp = fopen (_path, "w");
	if(!fp){
		return FILE_CANNOT_OPEN;
	}
	/*saving*/
	fprintf(fp,"%d %d\n", _game->rows, _game->cols);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			writeNumToFile(fp,_game->board,i,j,_game->mode);
			if (j == N-1){
				fputc('\n',fp);
			}
			else{
				fputc(' ', fp);
			}
		}
	}
	fclose(fp);
	printf("Saved to: %s\n", _path);
	return ERR_OK;
}

ADTErr solve(Game* _game, char* _path){
	return ERR_OK;
}


ADTErr reset (Game* _game){
	return ERR_OK;
}

ADTErr change_mark_errors (Game* _game , bool _x){
	_game->mark_error = _x;
	return ERR_OK;
}

ADTErr edit (Game* _game, char* _parsed_command){
	FILE* fp = NULL;
	ADTErr res = ERR_OK;
	if(_game->moveList != NULL){
		list_destroy(_game->moveList);
		_game->moveList = NULL;
		_game->moveList = malloc(sizeof(list));
		list_new(_game->moveList,sizeof(SingleSet),freeFuncSingleSet);
	}
	if(_parsed_command == NULL){ /* create empty board 9*9 */
		if(_game->board != NULL){
			free_board(_game);
		}
		_game->cols = DEF_COLS;
		_game->rows = DEF_ROWS;
		_game->board = create_empty_board(DEF_ROWS,DEF_COLS);
		res=ERR_OK;
	}
	else{
		fp = fopen (_parsed_command, "r");
		if(!fp){
			return FILE_CANNOT_OPEN;
		}
		res = parse_file(fp,_game);
		fclose(fp);
	}
	if(res==ERR_OK){
		_game->mode = EDIT;
	}
	return res;
}


ADTErr num_of_solutions (Game* _game){
	int num_solutions , _N;
	ADTErr err;
	_N = _game->cols * _game->rows;
	if ( ( err = erroneous_board ( _game-> board , _N ) ) != ERR_OK )
	{
		return err;
	}
	else{
		num_solutions = backtrack_Algo(_game->board, _game->rows , _game->cols);
		printf(" number of solutions is : %d", num_solutions);
	}
	return ERR_OK;
}

/*
 * LP
 */
ADTErr guess_hint(Game* _game, int _userRow, int _userCol){
	return generalHint(_game,_userRow,_userCol,FALSE);
}

ADTErr guess(Game* _game, float _x){
	return ERR_OK;
}

/**
 * print_dashes - prints 4N+m+1 dashes.
 *
 */
void print_dashes(int m, int n) {
	int i;
	for (i=0; i<4*n*m+m+1; i++) {
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
void print_num (Num n, MODE mode, int mark_errors) {

	switch (n.status) {
		case HIDDEN: /*value is 0*/
			printf("    ");
			break;
		case SHOWN:
			printf(" %2d ", n.num);
			break;
		case FIXED:
			if (mode == SOLVE)
				printf(" %2d.", n.num);
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


ADTErr printBoard(Game* _game){
	int row;
	int col;
	for (row = 0; row < _game->rows*_game->cols; row++){
		if (row % _game->rows == 0){
			print_dashes(_game->rows,_game->cols);
		}
		for (col = 0; col < _game->cols *_game->rows; col++) {
			if (col % _game->cols== 0)
				printf("%s","|");
			print_num(_game->board[row][col], _game->mode, _game->mark_error);
		}
		printf("%c\n",'|');
	}
	print_dashes(_game->rows,_game->cols);
	return ERR_OK;
}

ADTErr exit_game (Game* _game){
	game_destroy(_game);
	return EXIT;
}



bool validate_row(Num** board, int row, int col,int checked_value, int size){
	int i;
	for (i = 0; i< size; i++)
	{
		if (i != col)
		{
			if (board[row][i].num== checked_value)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

bool validate_col(Num** board, int row, int col, int checked_value, int size){
	int i;
	for (i = 0; i< size; i++)
	{
		if (i != row)
		{
			if (board[i][col].num == checked_value)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
	}

bool validate_block(Num** board, int row, int col, int checked_value, int block_rowsize, int block_colsize){
	int a, b, c, d;
	a = (row / block_rowsize) ;
	b = (col / block_colsize) ; /* the same with the col*/
	for (c = 0; c < block_colsize; c++)
	{ /*  iterates over the elements in the block */
		for (d = 0; d < block_rowsize; d++)
		{
			if ( !((a+c==row) &&(b+d==col)) )
			{
				if (board[a + c][b + d].num == checked_value)
				{
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

bool validate_digit (Num** board, int blockRow, int blockCol,int cell_row, int cell_col , int _dig) {
	int size;
	size=blockCol*blockRow;
	if (_dig == 0)
		return TRUE;
	if (!validate_row(board,cell_row, cell_col,_dig,size))
		return FALSE;
	if (!validate_col(board,cell_row, cell_col,_dig,size))
		return FALSE;
	if (!validate_block(board,cell_row, cell_col,_dig,blockRow,blockCol))
		return FALSE;
	return TRUE;
}

bool board_isSolvable(Num** board, int blockRow, int blockCol){
	int i,j;
	for(i=0; i<blockRow*blockCol;i++)
	{
		for(j=0; j<blockRow*blockCol;j++)
		{
			if(!validate_digit(board, blockRow, blockCol,i, j , board[i][j].num))
			{
			return FALSE;
			}
		}
	}return TRUE;
}
