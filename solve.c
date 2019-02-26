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

ADTErr solve_ilp(Game* _game){
	return ERR_OK;
}

ADTErr solve_lp(Game* _game){
	return ERR_OK;
}


bool validate_row(int _dig, int _row, Game* _game){
	return TRUE;
}

bool validate_col(int _dig, int _col, Game* _game){
	return TRUE;
}

bool validate_block(int _dig, int _row, int _col, Game* _game){
	return TRUE;
}

/**
* function checks if a placement of a given digit in a given cell is valid,
* @Input
* 	 int dig - the checked digit
* 	 int row - the cell's row
* 	 int col - the cell's column
* 	 Game* game

* @return
* FALSE - if one of the validations has failed
* TRUE - if all validations succeeded
*/
bool validate_dig (int _dig, int _row, int _col,Game* _game) {
	if (_dig == 0)
		return TRUE;
	if (!validate_row(_dig, _row, _game))
		return FALSE;
	if (!validate_col(_dig, _col, _game))
		return FALSE;
	if (!validate_block(_dig,_row,_col,_game))
		return FALSE;
	return TRUE;
}
