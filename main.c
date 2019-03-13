#include <stdio.h>

#include "macro.h"
#include "bool.h"
#include "ADT_Error.h"
#include "list.h"
#include "game.h"
#include "main_aux.h"
#include "parcer.h"

#include <stdlib.h>

void freeFuncStepsList2(void * _item){
	free((int*)_item);
}

bool iterate_int(void *data)
{
  printf("Found value: %d\n", *(int *)data);
  return TRUE;
}

int main () {
/*
	int i;
	int* item;
	int* item2;
	stack* l;
	l = (stack*)malloc(sizeof(stack));
	stack_new(l, sizeof(int*), freeFuncStepsList2);

	  for(i = 1; i <= 10; i++) {
		  item = malloc(sizeof(int));
		  *item = i;
		  stack_push(l, item);
	  }
	  item2 = malloc(sizeof(int));
	  *item2 = -1;
	  printf("%d\n", *item);
	  stack_pop(l,item);
	  stack_pop(l,item);
	  stack_pop(l,item);
	  list_for_each(l->list, iterate_int);
	  item2 = (int*)stack_peek(l);
	  printf("%d\n", *item2);

	stack_destroy(l);
*/


	int command_res = 1;
	Game* game = NULL;
	char commandStr[COMMAND_LEN];

	printf("Sudoku\n------\n");
	game = game_init();

	while(command_res > 0){
		printf("Enter your command:\n");
		if(mainAux_readCommand(commandStr) == TRUE){
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
