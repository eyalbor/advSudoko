
#include <stdio.h>
#include <stdlib.h>
#include "ADT_Error.h"

 void HandleErr(ADTErr errNum)
{
	 char* ad[]=
	{
		/* General ADT Errors */
		"exit",
		"ERR_OK",
		"ERR_GENERAL",
		"ERR_NOT_INITIALIZED",
		"ERR_ALLOCATION_FAILED",
		"ERR_REALLOCATION_FAILED",
		"ERR_UNDERFLOW",
		"ERR_OVERFLOW",
		"ERR_WRONG_INDEX",
		/* String Errors */
		"cell is fixed",
		"value is invalid",
		"HINT",
		"PUZZLE_SOLVED",
		"Validation passed: board is solvable",
		"Validation failed: board is unsolvable",
		"Invalid command",
		"Enter command",
		"Args invalid",
		"Too many parameters was entered",
		"Not enough parameters was entered",
		"Board is with errors",
		"Command is not available in current mode",
		"Cannot undo",
		"Cannot redo",
		"Not enough empty cell",
		"Board is not solved",
		"File cannot be opened",
		"File error"
		"Puzzle is not valid - cannot be saved",
		"Puzzle contain's error values",
		"Invalid range",
		"Puzzle contains value in this cell"

	};
	if(errNum==VALIDATION_PASSED){
		printf("%s\n", ad[errNum]);
	}
	else{
		printf("Error: %s\n", ad[errNum]);
	}
}

