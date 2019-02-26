#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "macro.h"
#include "bool.h"
#include "list.h"
#include "ADT_Error.h"
#include "game.h"
#include "parcer.h"

void freeFuncSingleSet(void * _item){
	free(_item);
}

void free_board(Game* _game){
	 int i;
	 int N = _game->cols *_game->rows;
	 for (i = 0; i < N; i++) {
		free(_game->board[i]); /*frees all cols*/
	}
	free(_game->board); /*frees all rows*/
}

Game* game_init(){
	Game* game = (Game*)malloc(sizeof(game));
	game->moveList = malloc(sizeof(list));
	list_new(game->moveList,sizeof(SingleSet),freeFuncSingleSet);
	game->mark_error=TRUE;
	game->mode=INIT;
	return game;
}

void game_destroy(Game* _game){
	if(_game->board!=NULL){
		free(_game->board);
	}
	list_destroy(_game->moveList);
	free(_game);
}

ADTErr validate (Game* _game){
	return ERR_OK;
}


/**
 * create_empty_board - creates a new empty board of the given size.
 * @param
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


ADTErr set ( Game* _game, int _col, int _row, int _dig){
	return ERR_OK;
}

/**
 * doing 1000 iteration else return ERROR
 */
ADTErr generate (Game* _game, int _x, int _y){
	return ERR_OK;
}

ADTErr undo (Game* _game){
	return ERR_OK;
}

ADTErr redo (Game* _game){
	return ERR_OK;
}

ADTErr autofill (Game* _game){
	return ERR_OK;
}

ADTErr hint (Game* _game, int _x, int _y){
	return ERR_OK;
}

ADTErr save (Game* _game, char* _path){
	return ERR_OK;
}

ADTErr solve(Game* _game, char* _path){
	return ERR_OK;
}


ADTErr reset (Game* _game){
	return ERR_OK;
}

ADTErr change_mark_errors (Game* _game , bool _x){
	return ERR_OK;
}

ADTErr edit (Game* _game, char* _parsed_command){
	FILE* fp;
	int res;
	if(_game->moveList!=NULL){
		list_destroy(_game->moveList);
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
	return ERR_OK;
}


ADTErr guess_hint(Game* _game, int _x, int _y){
	return ERR_OK;
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
 * print_num prints the numbers according to their status (HIDDEN|SHOWN|FIXED|ERRONEOUS) and according to game's mode.
 *
 * @Input
 * n - struct Num, which represents the number that appears in the cell.
 * mode - game's mode
 * mark_errors - indicates whether to mark erros
 */
void print_num (Num n, MODE mode, int mark_errors) {
	STAT stat = n.status;
	int number = n.num;
	switch (stat) {
	case HIDDEN: /*value is 0*/
		printf("    ");
		break;
	case SHOWN:
		printf(" %2d ", number);
		break;
	case FIXED:
		if (mode == SOLVE)
			printf(" %2d.", number);
		else
			printf(" %2d ", number);
		break;
	case ERRONEOUS:
		if (mode == EDIT || mark_errors)
			printf(" %2d*", number);
		else
			printf(" %2d ", number);
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
	return EXIT;
}


ADTErr validate_dig(dig,r,c,_game){
	return ERR_OK;
}
