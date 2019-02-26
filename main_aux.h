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
 * mainAux_getSeed
 * input:
 * 		int argc - number of string in argv
 * 		int argv - array of string
 * 	return: the seed for the next randomize functions, if not valid seed return -1
 */
int mainAux_getSeed(int argc,char** argv);

/**
 * mainAux_getNumberOfFixCells
 * extract from the user the number of fixed cells on the board
 * return: number of fixed cells. -1 if EOF error
 */
int mainAux_getNumberOfFixCells();

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
 * Output: z. value of cell
 */
void main_Aux_printHint(int z);

/*mainAux_printError
 * Input: ADTERR error. value of error.
 * Output: HandleErr function with the input error. it prints the error.
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
