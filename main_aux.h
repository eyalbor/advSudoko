/*
 * main_aux.h
 *
 *  Created on: Dec 11, 2018
 *      Author: eyal & nadin
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include "ADT_Error.h"
#include "game.h"

/**
 * mainAux_readCommand
 * input:
 * 		char* command - the value is filled by the input from the user
 * 	return: true if success otherwise false
 */
bool mainAux_readCommand(char* command);

/**
 * mainAux_printBoard
 * input: Game*
 * print the sudoku board to console
 */
void mainAux_printBoard(Game* _game);

/** mainAux_printExit
 * prints exiting
 */
void mainAux_printExit();

/** main_Aux_printHint
 * Input: int z : value of cell
 */
void main_Aux_printHint(int z);

/*mainAux_printError
 * Input: ADTERR error. value of error.
 * it prints the error.
 */
void mainAux_printError(ADTErr err);

/*
 * mainAux_printHint
 * Input: int dig
 * Output: void
 */
void mainAux_printHint(int dig);

/*
 * mainAux_printGameFinish
 * Input:
 * Output:
 */
void mainAux_printGameFinish();

#endif
