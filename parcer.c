#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "bool.h"
#include "list.h"
#include "game.h"
#include "parcer.h"
#include "macro.h"
#include "main_aux.h"



char delimit[]=" \t\r\v\f\n";

bool isNumber(char* input){
	int i;
	int ln;

	ln = strlen(input) - 1;
	/* Ensure that input is a number */
	for(i = 0; i < ln; i++){
	    if( !isdigit(input[i]) ){
	        return FALSE;
	    }
	}
	return TRUE;
}

/**
* using the function strtok, the function checks if the next token in command is legal.
* if token isn't legal, function prints an error message. if token is legal, function returns it. (a pointer to the first character of token)
*
* @param parsed_command - the current token.
* @return
* 0 - if next token got from strtok == NULL
* parsed_command (the next token) - otherwise
*/
char* check_next_tok(char* parsed_command) {
	parsed_command = strtok(NULL,delimit);
	if (parsed_command == NULL) {
//		print_invalid();
		return NULL;
	}
	return parsed_command;
}

/**
 * check_integer:
 * @param
 * num - a double
 * @return
 * 1 if num is integer (its fraction is 0)
 * 0 otherwise
 */
int check_integer(double num) {
	if (num - (int) num == 0)
		return 1;
	return 0;
}

/**
 * read_args - reads the specified number of arguments from the given command and checks if they are legal.
 * function only checks proper number of arguments and integer value of them when needed.
 * if number is not integer, function changes it to -1.
 * function updates the values of parameters. prints error messages if values aren't legal.
 * @param
 * parsed_command - the given command
 * args_num - number of arguments of the proper command
 * x - pointer to variable x
 * y - pointer to variable y
 * z - pointer to variable z
 * func - name of calling function
 * @return
 * 1 - if command is legal or number is not integer
 * 0 - otherwise (illegal command)
 */
int read_args(char** parsed_command, int args_num, double* x, double* y, double* z, char* func) {
	*parsed_command = check_next_tok(*parsed_command);
	if (*parsed_command == NULL)
		return 0;
	*x = atof(*parsed_command);
	if (args_num > 1) {
		*parsed_command = check_next_tok(*parsed_command);
		if (*parsed_command == NULL)
			return 0;
		*y = atof(*parsed_command);
	}
	if (args_num > 2) {
		*parsed_command = check_next_tok(*parsed_command);
		if (*parsed_command == NULL)
			return 0;
		*z = atof(*parsed_command);
	}
	if ((!check_integer(*x) || (*x == 0.0 && **parsed_command != '0')) && strcmp(func,"solve") != 0 && strcmp(func,"save") != 0)
		*x = -1;
	if (args_num > 1 && (!check_integer(*y) || (*y == 0.0 && **parsed_command != '0')))
		*y = -1;
	if (args_num > 2 && (!check_integer(*z) || (*z == 0.0 && **parsed_command != '0')))
		*z = -1;
	return 1;
}


/**
 * Command are case sensitive
 * TODO to check if command available in the current mode before check vars
 * return: if the user enter invalid command return INVALID_COMMAND
 * 		if use command with extra parameters return TOO_MANY_PARMS
 * 		if use command with not enough parameters return FEW_PARAMS
 * 		if the parameters not valid return ARGS_INVALID
 */
ADTErr parcer_doCommand(Game* _game, char* _command){
	char* tokens;
	ADTErr ret = INVALID_COMMAND;
	double x = 0, y = 0, z = 0;

	tokens = strtok(_command,delimit);

	if(tokens != NULL)
	{
		if (!strcmp(tokens,"validate")) {
			ret = validate(_game);
		}
		else if (!strcmp(tokens,"reset")) {
			ret = reset(_game);
		}
		else if (!strcmp(tokens,"hint")) {
			if (read_args(&tokens, 2, &x,&y,&z,"hint")!=ERR_OK) {
				return ARGS_INVALID;
			}
			ret = hint(_game,(int)x, (int)y);
		}
		else if (!strcmp(tokens,"set")) {
			if (read_args(&tokens, 3, &x,&y,&z,"set")!=ERR_OK) {
				return ARGS_INVALID;
			}
			ret = set(_game, (int)x,(int)y, (int)z);
		}
		else if (!strcmp(tokens,"solve")) {
			if (read_args(&tokens, 1, &x,&y,&z,"solve")!=ERR_OK) {
				return ARGS_INVALID;
			}
			ret = solve(_game, tokens);
		}
		else if (!strcmp(tokens,"edit")) {
			tokens = strtok(NULL,delimit);
			ret = edit(_game, tokens);
		}
		else if (!strcmp(tokens,"save")) {
			if (read_args(&tokens, 1, &x,&y,&z,"save") != ERR_OK)
				return ARGS_INVALID;
			ret = save(_game,tokens);
		}
		else if (!strcmp(tokens,"guess")) {
			if (read_args(&tokens, 1, &x,&y,&z,"guess") != ERR_OK)
				return ARGS_INVALID;
			ret = guess(_game,x);
		}
		else if (!strcmp(tokens,"generate")) {
			if (read_args(&tokens, 2, &x,&y,&z,"gen") != ERR_OK)
				return ARGS_INVALID;
			ret = generate(_game, (int) x, (int) y);
		}
		else if (!strcmp(tokens,"autofill")){
			ret = autofill(_game);
		}
		else if (!strcmp(tokens,"mark_errors")) {
			if (read_args(&tokens, 1, &x,&y,&z,"mark_errors") != ERR_OK)
				return ARGS_INVALID;
			ret = change_mark_errors(_game, (bool)x);
		}
		else if (!strcmp(tokens,"guess_hint")) {
			if (read_args(&tokens, 2, &x,&y,&z,"guess_hint") != ERR_OK)
				return ARGS_INVALID;
			ret = guess_hint(_game, (int) x, (int) y);
		}
		else if (!strcmp(tokens,"print_board")){
			mainAux_printBoard(_game);
			ret = ERR_OK;
		}
		else if (!strcmp(tokens,"undo"))
			ret = undo(_game);
		else if (!strcmp(tokens,"redo"))
			ret = redo(_game);
		else if (!strcmp(tokens,"num_solutions"))
			ret = num_of_solutions(_game);
		else if (!strcmp(tokens,"exit"))
			ret = exit_game(_game);
		else {
			ret = INVALID_COMMAND;
		}
	}
	return ret;
}

/**
 *
 */
ADTErr parse_file (FILE* fp, Game* _game){
	return ERR_OK;
}
