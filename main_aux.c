#include <stdio.h>
#include <stdlib.h>

#include "bool.h"
#include "macro.h"
#include "ADT_Error.h"
#include "game.h"
#include "main_aux.h"




bool mainAux_readCommand(char* command){
	fseek(stdin,0,SEEK_END);
	if (NULL == fgets(command,COMMAND_LEN,stdin)){
		return FALSE;
	}
	return TRUE;
}

/**
 * each cell contain 4 char:
 * 		space char, two digits(printf "%2d") & blank is two space, '.' for fixed otherwith space, '*' for error otherwith space
 */
void mainAux_printBoard(Game* _game){
	printBoard(_game);
}

void mainAux_printExit(){
	printf("Exiting...\n");
}

void mainAux_printError(ADTErr err){
	HandleErr(err);
}
