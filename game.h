#ifndef GAME_H_
#define GAME_H_

#include "list.h"
#include "ADT_Error.h"

/**
* Type represents game mode
*/
typedef enum game_mode {
	INIT,
	SOLVE, /* user can only enter number that in cell not fixed */
	EDIT /* no cell consider fixed */
} MODE;

/**
* Type represents cell status
*/
typedef enum num_status {
	HIDDEN,
	SHOWN,
	FIXED,
	ERRONEOUS
} STAT;

/**
* Type represents cell
*/
typedef struct sudoku_number {
	int num;
	STAT status;
} Num;

/**
* Type represents a single cell 'set' (change of cell's value).
*/
typedef struct set_cell {
	int prev_val; /*previous value of cell*/
	int new_val; /*new value of cell*/
	int col;
	int row; /*coordinates of cell*/
	STAT prev_stat;
	STAT new_stat;
} SingleSet;

typedef struct _game{
	MODE mode;
	bool mark_error; /* determine whether errors in the sudoku are display */
	list* moveList;
	Num** board;
	int rows;
	int cols;
} Game;

/****************************************** Functions **************************************************/

/**
 * game_init
 * @Input:
 * @Return:
 * 	Game*
 *
 * The program start on Init mode
 * user can load a file for Edit or Solve mode
 * Default to mark_error is TRUE
 * The redo/undo list is empty
 */
Game* game_init();


/** copy boards- receive a board and duplicate it.
 * @param
 * -old board
 * int _m- size of rows in a block
 * int _n- size of cols in a block
 * @return
 * new board **/
Num** copy_boards ( Num** old_board , int _m, int _n);


/**
 * game_destroy
 * @input
 * 	Game*
 *
 * 	clear all resources
 */
void game_destroy(Game* _game);

/**
 * solve:
 * 	The user try to solve pre-exiting puzzle
 * 	start in Solve MODE
 * 	load a file from _path
 *
 *  any unsaved work from current work is lost
 *
 * @Input:
 * 	Game*
 * 	char* path to file
 * 	@Return:
 * 		ADTErr ERR_OK
 * 		ADTErr FILE_CANNOT_OPEN
 *
 * MODE AVAILIBALE: INIT, SOLVE, EDIT
 *
 */
ADTErr solve(Game* _game, char* _path);

/**
 * edit
 *
 * start a puzzle in EDIT mode
 *
 * The user edit existing puzzle or create new ones and save this for opening in the Solve mode
 * mark error is always TRUE
 *
 * any unsaved work from current work is lost
 *
 *
 * @Input:
 * 	Game*
 * 	char* _pathToFile is optional, if NULL the program enter EDIT mode with empty 9X9 board
 * 	@Return:
 * 		ADTErr ERR_OK
 * 		ADTErr FILE_CANNOT_OPEN
 *
 * MODE AVAILIBALE: INIT, SOLVE, EDIT
 *
 */
ADTErr edit (Game* _game, char* _pathToFile);

/**
 * change_mark_errors
 *  if _x is True(1), erroneous values are displayed followed by *
 * * @Input:
 * 		Game*
 * 		bool _x
 * 	@Return: ERR_OK
 *
 * MODE AVAILIBALE: SOLVE
 *
 */
ADTErr change_mark_errors (Game* _game , bool _x);

/**
 * printBoard
 * 	print the sudoku board to console
 * input:
 * 		Game*
 *
 * @Return: ERR_OK
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr printBoard(Game* _game);

/**
 * set
 *
 * set the value of cell <_row,_col> with _dig
 * if the _dig is zero the cell is cleared
 * The redo part are cleared
 * cell can be mark
 *
 * @Input:
 * 		Game*
 * 		int _col
 * 		int _row
 * 		int _dig
 * 	@Return: ADTErr
 * 		INVALID_RANGE _dig base is invalid or _col || _row invalid
 * 		CELL_FIX - fix cell is not update in SOLVE mode
 * 		BOARD_ERRORS - if the last cell is filled then the board can be error return BOARD_ERRORS, the user must undo the move
 * 		PUZZLE_SOLVED - if the puzzle finish correct then the mode of the game set to INIT
 *
 * edit mode we mark the error regardless of the value markerror
 * solve can input error and show the error if mark error is true=1;
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr set ( Game* _game, int _col, int _row, int _dig);

/**
 * validate  - validate using ILP
 * @Input:
 * 		Game*
 * 	@Return:
 * 		ADTErr
 * 			BOARD_ERRORS - if the board is with error the program return error message
 * 			ERR_OK - board is solvable
 *
 * 	MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr validate (Game* _game);

/*
 * guess
 *
 * guess solution to the board using LP.
 * 	The command fills all the cell with legal values with a score in LP of _x or more.
 * 	if several values hold for the same cell choose one according to the score
 * 	The redo part are cleared
 *
* @Input:
* 		Game*
* 		float _x is threshold for LP
* @Return:
* 		ADTErr
* 		BOARD_ERRORS if the board is with error the program return error message
* 		ERR_OK - board is solvable
*
* 	MODE AVAILIBALE: SOLVE
*/
ADTErr guess(Game* _game, float _x);

/**
 * generate
 * fill puzzle by randomly filling _x empty cells with legal values
 * run ILP
 * clear all by Y random cells
 * The redo part are cleared
 *
* @Input:
* 		Game*
* 		int _x
*		int _y
* @Return:
* 		ADTErr
* 			if the board not contain _x empty cell return NOT_ENOUGH_EMPTY_CELL
* 			BOARD_ERRORS if the board is with error the program return error message
* 			ERR_OK - board is solvable
*
* MODE AVAILIBALE: EDIT
 */
ADTErr generate (Game* _game, int _x, int _y);

/**
 * undo - undo move
 * 	print the change that was made
 *@ Input:
 * 		Game*
  @ Return if there is not move to redo return CANNOT_UNDO
  	  ERR_OK - all ok
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr undo (Game* _game);

/**
 * redo - redo move
 * print the change that was made
 	 @Input:
 * 		Game*
 * @Return if there us not move to redo return CANNOT_REDO
 * 		ERR_OK - all ok
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr redo (Game* _game);

/**
 * autofill = fill all empty cells contain only one legal value
 @Input:
 * 		Game*
 * @Return
 * 	BOARD_ERRORS - if board is erroneous
 * 	ERR_OK if board is valid print changes message
 *
 * MODE AVAILIBALE: SOLVE
 *
 */
ADTErr autofill (Game* _game);

/**
 * hint
 *  give hint to user of cell <_x,_y>
 * 	run ILP to solve a board
 *
 * @Input:
 * 	Game*
 * 	int _x
 * 	int _y
 *
 * 	@Return:
 * 		INVALID_RANGE of x or y,
 * 		CELL_FIX - if cell is fixed
 * 		CELL_HAVE_VALUE - cell contain a value
 * 		BOARD_ERRORS - if the board not solve
 * 		ERR_OK -otherwise print the user the value
 *
 * 	MODE AVAILIBALE: SOLVE
 */
ADTErr hint (Game* _game,  int _userRow, int _userCol);

/**
 * save
 *
 *
 ** Edit mode: the board have to be validate, all filled cells are marked as fixed,
 * 			puzzle with Error cannot be save in Edit mode.
 * 			board without solution cannot be saved
 * 	Solve mode: puzzle with Error can be save!
 *
 * @Input:
 * 	Game*
 * 	char* _path to file
 * 	@Return:
 * 		ADTErr
 * 			BOARD_ERRORS
 * 			ERR_OK
 * 			FILE_ERROR
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr save (Game* _game, char* _path);

/**
 * num_of_solutions
 * @Input:
 * 	Game*
 * 	@Return:
 * 		ADTErr if the board is error return BOARD_IS_NOT_SOLVED
 *
 * 	print number of solution to board
 * use exhaustive backracking and stack for recursive
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr num_of_solutions (Game* _game);

/**
 * guess_hint
 *
 * show a guess to the user for single cell <_x,_y>
 * run LP
 *
 * @Input:
 * 	Game*
 * 	int_x
 * 	int _y
 * 	@Return:
 * 		INVALID_RANGE,
 * 		CELL_FIX - if cell is fixed
 * 		CELL_HAVE_VALUE - cell contain a value
 * 		BOARD_ERRORS - if the board not solve
 * 		ERR_OK -otherwise print the user the value
 *
 * MODE AVAILIBALE: SOLVE
 */
ADTErr guess_hint(Game* _game, int _userRow, int _userCol);

/**
 * reset
 *
 *  undo all moved, reverting the board to it original loaded state
 *  the move list is not cleared!
 *
 *  @Input:
 * 		Game*
 * @Return ADTErr
 * 		ERR_OK
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr reset (Game* _game);

/**
 * exit
 * @Input:
 * 		Game*
 * @Return ADTErr
 *
 * exit the program, clear all the resources!  * any unsave work is lost
 * MODE AVAILIBALE: INIT, SOLVE, EDIT
 *
 * @Return ADTErr
 * 		ERR_OK
 *
 */
ADTErr exit_game (Game* _game);

/**
 * create_empty_board - creates a new empty board of the given size.
 * @param
 * m - number of rows in one block
 * n - number of columns in one block
 */
Num** create_empty_board(int _m, int _n);

void printBoard2(Num** board, int Brow, int Bcol);

bool validate_digit(Num** board, int blockRow, int blockCol,int cell_row, int cell_col , int _dig);

bool board_isSolvable(Num** board, int blockRow, int blockCol);

#endif
