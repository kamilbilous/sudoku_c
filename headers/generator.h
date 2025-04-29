#ifndef GENERATOR_H
#define GENERATOR_H

#include "board.h"
#include <stdbool.h>

Board* generate_board(int size); //Creates a filled board
bool remove_cells(Board* full_board, int k); //Removes k cells from the full board
bool solve_board(Board* board); // Solves the board using backtracking
int count_solutions(Board* board); //Counts possible solutions, so the board generates with only 1 solution
#endif