#ifndef SOLVE_H_
#define SOLVR_H_

/**
 * solve_ilp - solves Sudoku board with ILP using Gurobi.
 * @Input
 * 	_game
 * int _userRow,
 * int _userCol,
 * int* _result,
 * int func : 0 - hint, 1 - validate , 2 - generate
 * @Return ADTErr
 */
ADTErr solve_ilp(Game* _game, int _userRow, int _userCol, int* _result, int func);

/**
 * solve_lp - solves Sudoku board with LP
 * @Input
 * 	_game
 * @Return ADTErr
 */
ADTErr solve_lp(Game* _game);

int backtrack_Algo(Num** current_board, int block_row, int block_col);

#endif

