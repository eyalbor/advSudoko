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

/** eyal i just put this functions meanwhile until we see how to build the ilp function. we will probably need
 * only one function each but as i dont want to change the entire code now and make adjustments i leaving it like this
 * until we finish the ilp function. **/
ADTErr solve_ilpDuplicated(Game* _game, Num** temp_board);

ADTErr solve_lpDuplicated(Game* _game, Num** temp_board);

int backtrack_Algo(Num** current_board, int block_row, int block_col);

#endif

