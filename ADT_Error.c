
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
		"invalid command",
		"enter command",
		"Args invalid",
		"too many parameters was entered",
		"not enough parameters was entered",
		"board is with errors",
		"command is not available in current mode",
		"cannot undo",
		"cannot redo",
		"not enough empty cell",
		"board is not solved",
		"file cannot be opened"

	};
	if(errNum==VALIDATION_PASSED){
		printf("%s\n", ad[errNum]);
	}
	else{
		printf("Error: %s\n", ad[errNum]);
	}
}

