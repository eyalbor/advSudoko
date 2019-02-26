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

#endif

