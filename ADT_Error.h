/*
 * ADT_Error.h
 *
 *  Created on: Dec 11, 2018
 *      Author: eyal & nadin
 */

#ifndef ADTERROR_H_
#define ADTERROR_H_

typedef enum
{
    /* General ADT Errors */
    ERR_OK                            =0,
    ERR_GENERAL,
    ERR_NOT_INITIALIZED,
    ERR_ALLOCATION_FAILED,
    ERR_REALLOCATION_FAILED,
    ERR_UNDERFLOW,
    ERR_OVERFLOW,
    ERR_WRONG_INDEX,

    /* String Errors */
	CELL_FIX,
	VALUE_INVALID,
	HINT_ERR,
	PUZZLE_SOLVED,
	VALIDATION_PASSED,
	VALIDATION_FAILED,
	INVALID_COMMAND,
	ENTER_COMMAND,
	ARGS_INVALID,
	TOO_MANY_PARMS,
	FEW_PARAMS,
	BOARD_ERRORS,
	COMMAND_NOT_AVAILABLE_INCURRENT_MODE,
	CANNOT_UNDO,
	CANNOT_REDO,
	NOT_ENOUGH_EMPTY_CELL,
	BOARD_IS_NOT_SOLVED

} ADTErr;

/**
 * HandleErr
 * print the error
 * input: ADTErr error
 */
void HandleErr(ADTErr errNum);

#endif /*ADTERROR_H_*/
