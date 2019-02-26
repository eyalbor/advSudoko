#ifndef SOLVE_H_
#define SOLVR_H_

/**
 * solve_ilp - solves Sudoku board with ILP using Gurobi.
 * @Input
 * 	_game
 * @Return ADTErr
 */
ADTErr solve_ilp(Game* _game);

/**
 * solve_lp - solves Sudoku board with LP
 * @Input
 * 	_game
 * @Return ADTErr
 */
ADTErr solve_lp(Game* _game);

bool validate_dig (int _dig, int _row, int _col, Game* _game);

#endif

