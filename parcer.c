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

/**
 * INIT=0, SOLVE=1, EDIT=2
 */
bool solveM[] = {1, 1, 1};
bool editM[] = {1, 1, 1};
bool change_mark_errorsM[] = {0, 1, 0};
bool printBoardM[]={0,1,1};
bool setM[]={0,1,1};
bool validateM[] = {0,1,1};
bool guessM[] ={0,1,0};
bool generateM[]={0,0,1};
bool undoM[]={0,1,1};
bool redoM[]={0,1,1};
bool autofillM[]={0,1,0};
bool hintM[]={0,1,0};
bool saveM[]={0,1,1};
bool num_of_solutionsM[]={0,1,1};
bool guess_hintM[]={0,1,0};
bool resetM[]={0,1,1};
bool exitM[] = {1, 1, 1};

ADTErr checkMode(bool cmdM[3], MODE mode){
	if(cmdM[mode]==TRUE){
		return ERR_OK;
	}
	return COMMAND_NOT_AVAILABLE_INCURRENT_MODE;
}

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
 * check_next_tok
* checks if the next token in command is legal.
* @Input
* 	parsed_command - the current token.
* @return char*  - the next tokens
*/
char* check_next_tok(char* parsed_command) {
	parsed_command = strtok(NULL,delimit);
	if (parsed_command == NULL) {
		return NULL;
	}
	return parsed_command;
}

/**
 * check_integer:
 * @Input
 * 		num double
 * @return
 * 		TRUE if num is integer (its fraction is 0)
 * 		FALSE otherwise
 */
bool check_integer(double num) {
	if (num - (int) num == 0)
		return TRUE;
	return FALSE;
}

/**
 * read_args -
 * reads the specified number of arguments from the given command and checks if they are legal.
 *
 * @Input
 * 	char* parsed_command - the given command
 * 	args_num - number of arguments of the proper command
 * 	x - pointer to variable x
 * 	y - pointer to variable y
 * 	z - pointer to variable z
 * 	func - name of calling function
 * @return ADTErr
 * 	ERR_OK - legal command
 * 	ARGS_INVALID - otherwise
 */
ADTErr read_args(char* parsed_command, int args_num, double* x, double* y, double* z, char* func) {
	parsed_command = check_next_tok(parsed_command);
	if (parsed_command == NULL)
		return ARGS_INVALID;

	if(strcmp("mark_errors",func)==0){
		if((*parsed_command!='0' || *parsed_command!='1'))
			return ARGS_INVALID;
		else{
			/* atof converts the string argument str to a floating-point number */
			*x = atof(parsed_command);
			return ERR_OK;
		}
	}

	*x = atof(parsed_command);
	if(*x == 0.0){
		return ARGS_INVALID;
	}

	if (args_num > 1) {
		parsed_command = check_next_tok(parsed_command);
		if (parsed_command == NULL)
			return ARGS_INVALID;
		*y = atof(parsed_command);
		if(*y == 0.0){
			return ARGS_INVALID;
		}
	}
	if (args_num > 2) {
		parsed_command = check_next_tok(parsed_command);
		if (parsed_command == NULL)
			return ARGS_INVALID;
		*z = atof(parsed_command);
		if(*z == 0.0){
			return ARGS_INVALID;
		}
	}
	return ERR_OK;
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
			if((ret=checkMode(validateM,_game->mode))==ERR_OK){
				ret = validate(_game);
			}
		}
		else if (!strcmp(tokens,"reset")) {
			if((ret=checkMode(resetM,_game->mode))==ERR_OK){
				ret = reset(_game);
			}
		}
		else if (!strcmp(tokens,"hint")) {
			if (read_args(tokens, 2, &x,&y,&z,"hint")!=ERR_OK) {
				return ARGS_INVALID;
			}
			if((ret=checkMode(hintM,_game->mode))==ERR_OK){
				ret = hint(_game,(int)x, (int)y);
			}
		}
		else if (!strcmp(tokens,"set")) {
			if (read_args(tokens, 3, &x,&y,&z,"set")!=ERR_OK) {
				return ARGS_INVALID;
			}
			if((ret=checkMode(setM,_game->mode))==ERR_OK){
				ret = set(_game, (int)x,(int)y, (int)z);
			}
		}
		else if (!strcmp(tokens,"solve")) {
			if (read_args(tokens, 1, &x,&y,&z,"solve")!=ERR_OK) {
				return ARGS_INVALID;
			}
			if((ret=checkMode(solveM,_game->mode))==ERR_OK){
				ret = solve(_game, tokens);
			}
		}
		else if (!strcmp(tokens,"edit")) {
			tokens = strtok(NULL,delimit);
			if((ret=checkMode(editM,_game->mode))==ERR_OK){
				ret = edit(_game, tokens);
			}
		}
		else if (!strcmp(tokens,"save")) {
			tokens = strtok(NULL,delimit);
			if((ret=checkMode(saveM,_game->mode))==ERR_OK){
				ret = save(_game,tokens);
			}
		}
		else if (!strcmp(tokens,"guess")) {
			if (read_args(tokens, 1, &x,&y,&z,"guess") != ERR_OK)
				return ARGS_INVALID;
			if((ret=checkMode(guessM,_game->mode))==ERR_OK){
				ret = guess(_game,x);
			}
		}
		else if (!strcmp(tokens,"generate")) {
			if (read_args(tokens, 2, &x,&y,&z,"gen") != ERR_OK)
				return ARGS_INVALID;
			if((ret=checkMode(generateM,_game->mode))==ERR_OK){
				ret = generate(_game, (int) x, (int) y);
			}
		}
		else if (!strcmp(tokens,"autofill")){
			if((ret=checkMode(autofillM,_game->mode))==ERR_OK){
				ret = autofill(_game);
			}
		}
		else if (!strcmp(tokens,"mark_errors")) {
			if (read_args(tokens, 1, &x,&y,&z,"mark_errors") != ERR_OK)
				return ARGS_INVALID;
			if((ret=checkMode(change_mark_errorsM,_game->mode))==ERR_OK){
				ret = change_mark_errors(_game, (bool)x);
			}
		}
		else if (!strcmp(tokens,"guess_hint")) {
			if (read_args(tokens, 2, &x,&y,&z,"guess_hint") != ERR_OK)
				return ARGS_INVALID;
			if((ret=checkMode(guess_hintM,_game->mode))==ERR_OK){
				ret = guess_hint(_game, (int) x, (int) y);
			}
		}
		else if (!strcmp(tokens,"print_board")){
			if((ret=checkMode(printBoardM,_game->mode))==ERR_OK){
				mainAux_printBoard(_game);
				ret = ERR_OK;
			}
		}
		else if (!strcmp(tokens,"undo")){
			if((ret=checkMode(undoM,_game->mode))==ERR_OK){
				ret = undo(_game);
			}
		}
		else if (!strcmp(tokens,"redo")){
			if((ret=checkMode(redoM,_game->mode))==ERR_OK){
				ret = redo(_game);
			}
		}
		else if (!strcmp(tokens,"num_solutions")){
			if((ret=checkMode(num_of_solutionsM,_game->mode))==ERR_OK){
				ret = num_of_solutions(_game);
			}
		}
		else if (!strcmp(tokens,"exit")){
			if((ret=checkMode(exitM,_game->mode))==ERR_OK){
				ret = exit_game(_game);
			}
		}
		else {
			ret = INVALID_COMMAND;
		}
	}
	return ret;
}

/**
 * fp is already open
 * all the data can be in a single line
 */
ADTErr parse_file (FILE* fp, Game* _game){
	char line[COMMAND_LEN];
	char* read_tok;
	int r, c, N, dig;
	int count = -2;
	ADTErr err;

	while (fgets(line, COMMAND_LEN, fp) != NULL) {
		if (ferror(fp)) { /* fgets error*/
			printf("Error: fgets has failed\n");
			return EXIT;
		}
		read_tok = strtok(line, delimit);
		while(read_tok != NULL){

			if(isNumber(read_tok) == FALSE){
				return EXIT;
			}
			if (count == -2){
				_game->rows = atoi(read_tok);
			}
			else if (count == -1){
				_game->cols = atoi(read_tok);
			}
			else {
				if(count == 0){
					/* init board */
					N = _game->rows * _game->cols;
					_game->board = create_empty_board(_game->rows, _game->cols);
				}
				r = count/N;
				c = count%N;
				dig = atoi(read_tok);
				if(dig != 0){
					err = validate_digit(dig,r,c,_game);
					if (read_tok[strlen(read_tok)-1] == '.' && _game->mode == SOLVE){
						((_game->board)[r][c]).status = FIXED;
					}
					else if ( err != ERR_OK){
						((_game->board)[r][c]).status = ERRONEOUS;
					}
					else{
						((_game->board)[r][c]).status = SHOWN;
					}
					((_game->board)[r][c]).num = dig;
				}
			}
			count++;
			read_tok = strtok(NULL, delimit);
		}
	}
	return ERR_OK;
}
