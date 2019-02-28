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
//	int alt_num;
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
} SingleSet;

typedef struct _game{
	MODE mode;
	bool mark_error; /* determine whether errors in the sudoku are display */
	list* moveList;
	listNode* currentStepNode;
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
 * Default for error is TRUE
 * The redo/undo list is empty
 */
Game* game_init();

/**
 * game_destroy
 * @input
 * 	Game*
 *
 * 	clear all resources
 */
void game_destroy(Game* _game);

/**
 * solve
 * @Input:
 * 	Game*
 * 	char* path to file
 * 	@Return:
 * 		ADTErr
 *
 * The user try to solve pre-exiting puzzle
 * start in Solve MODE
 * load a file from _path
 *
 * MODE AVAILIBALE: INIT, SOLVE, EDIT
 *
 * any unsaved work from current work is lost
 */
ADTErr solve(Game* _game, char* _path);

/**
 * edit
 * @Input:
 * 	Game*
 * 	char* _pathToFile is optional, if NULL the program enter EDIT mode with empty 9X9 board
 * 	@Return:
 * 		ADTErr
 *
 * 	start a puzzle in EDIT mode
 *
 * The user edit existing puzzle or create new ones and save this for opening in the Solve mode
 * mark error is always TRUE
 *
 * MODE AVAILIBALE: INIT, SOLVE, EDIT
 *
 * any unsaved work from current work is lost
 */
ADTErr edit (Game* _game, char* _pathToFile);

/**
 * change_mark_errors
 * * @Input:
 * 		Game*
 * 		bool _x
 * 	@Return:
 * 		ADTErr
 *
 * if _x is True(1), erroneous values are displayed followed by *
 *
 * MODE AVAILIBALE: SOLVE
 *
 */
ADTErr change_mark_errors (Game* _game , bool _x);

/**
 * printBoard
 * input:
 * 		Game*
 * print the sudoku board to console
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
 *
 * @Input:
 * 		Game*
 * 		int _col
 * 		int _row
 * 		int _dig
 * 	@Return:
 * 		ADTErr
 * 		INVALID_RANGE _dig base is invalid or _col || _row invalid
 * 		CELL_FIX -fix cell is not update in SOLVE mode
 * 		cell can be mark as error if not correct
 * 		BOARD_ERRORS - if the last cell if filled then the board can be error return BOARD_ERRORS, the user must undo
 * 		PUZZLE_SOLVED - if the puzzle finish correct then the mode of the game set to INIT
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr set ( Game* _game, int _col, int _row, int _dig);

/**
 * validate
 * @Input:
 * 		Game*
 * 	@Return:
 * 		ADTErr if the board is with error the program return error message
 *
 * 	validate using ILP
 * 	MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr validate (Game* _game);

/*
 * guess
* @Input:
* 		Game*
* 		float _x is threshold for LP
* @Return:
* 		ADTErr if the board is with error the program return error message
*
* 	guess solution to the board using LP.
* 	The command fills all cell legal values with a score of _x or more.
* 	The redo part are cleared
* 	MODE AVAILIBALE: SOLVE
*/
ADTErr guess(Game* _game, float _x);

/**
 * generate
* @Input:
* 		Game*
* 		int _x
*		int _y
* @Return:
* 		ADTErr if the board not contain _x empty cell return NOT_ENOUGH_EMPTY_CELL
*
*	generete a puzzle by randomly filling _x empty cells with legal values
*	run ILP
*	clear all by Y random cells
*
* The redo part are cleared
* MODE AVAILIBALE: EDIT
 */
ADTErr generate (Game* _game, int _x, int _y);

/**
 * undo
 *@Input:
 * 		Game*
 *@Return if there us not move to redo return CANNOT_UNDO
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr undo (Game* _game);

/**
 * redo
 @Input:
 * 		Game*
 * @Return if there us not move to redo return CANNOT_REDO
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 */
ADTErr redo (Game* _game);

/**
 * autofill
 @Input:
 * 		Game*
 * @Return
 *
 * MODE AVAILIBALE: SOLVE
 *
 * TODO NOT understand- eyal
 */
ADTErr autofill (Game* _game);

/**
 * hint
 * @Input:
 * 	Game*
 * 	int _x
 * 	int _y
 *
 * 	@Return:
 * 		INVALID_RANGE,
 * 		CELL_FIX - if cell is fixed
 * 		CELL_HAVE_VALUE - cell contain a value
 * 		BOARD_ERRORS - if the board not solve
 * 		ERR_OK -otherwise print the user the value
 *
 * 	give hint to user of cell <_x,_y>
 * 	run ILP to solve a board
 *
 * 	MODE AVAILIBALE: SOLVE
 */
ADTErr hint (Game* _game,  int _userRow, int _userCol);

/**
 * save
 *
 * @Input:
 * 	Game*
 * 	char* _path to file
 * 	@Return:
 * 		ADTErr
 *
 *
 * MODE AVAILIBALE: SOLVE, EDIT
 * Edit mode: the board is validate, all filled cells are marked as fixed,
 * 			puzzle with Error cannot be save in Edit mode.
 * 			board without solution cannot be saved
 * 			when save all cell containing values are save as fixed
 * 	Solve mode: puzzle with Error can be save!
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
 *  @Input:
 * 		Game*
 * @Return ADTErr
 *
 * undo all moved, reverting the board to it original loaded state
 * MODE AVAILIBALE: SOLVE, EDIT
 * the move list is not cleared!
 */
ADTErr reset (Game* _game);

/**
 * exit
 * @Input:
 * 		Game*
 * @Return ADTErr
 *
 * exit the program, clear all the resources!
 * MODE AVAILIBALE: INIT, SOLVE, EDIT
 * any unsave work is lost
 */
ADTErr exit_game (Game* _game);

/**
 * create_empty_board - creates a new empty board of the given size.
 * @param
 * m - number of rows in one block
 * n - number of columns in one block
 */
Num** create_empty_board(int _m, int _n);

bool validate_digit(int dig, int r, int c, Game* _game);

#endif
