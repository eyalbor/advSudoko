#include <stdio.h>

#include "macro.h"
#include "bool.h"
#include "ADT_Error.h"
#include "main_aux.h"
#include "game.h"
#include "parcer.h"



int main (int argc, char** argv) {

	int command_res = 1;
	Game* game = NULL;
	char commandStr[COMMAND_LEN];

	printf("Sudoku\n------\n");
	game = game_init();

	while(command_res > 0){
		printf("Enter your command:\n");
		if(mainAux_readCommand(commandStr) == TRUE){
			//printf("%s",commandStr);
			command_res = parcer_doCommand(game,commandStr);
			if(command_res != EXIT && command_res!=ERR_OK){
				mainAux_printError((ADTErr)command_res);
			}
			else if(command_res == EXIT){
				mainAux_printError(EXIT);
			}
			else{
				mainAux_printBoard(game);
			}
		}
		else{
			mainAux_printError(ENTER_COMMAND);
		}
	}

	game_destroy(game);
	mainAux_printExit();
	return 0;
}
