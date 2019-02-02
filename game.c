#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "bool.h"
#include "list.h"
#include "ADT_Error.h"
#include "game.h"
#include "parcer.h"

void freeFuncSingleSet(void * _item){
	free(_item);
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

Num** create_empty_board(int _m, int n){
	return NULL;
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
	return ERR_OK;
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

ADTErr printBoard(Game* _game){
	return ERR_OK;
}

ADTErr exit_game (Game* _game){
	return ERR_OK;
}

