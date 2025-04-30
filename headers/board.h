#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

typedef struct {
    int size;
    int* cells; //size x size array
}Board;

Board* board_create(int size);
void board_free(Board* board); //Frees memory allocated for the board
void board_print(Board* board);
bool board_set(Board* board, int row, int col,int val); // Sets a value (1 to size) at the specified row and column. Returns false if invalid
int board_get(const Board* board, int row, int col); //Returns a value from a cell (0 = empty)
bool cell_is_valid(const Board* board, int row, int col, int val); //Returns True if placing val at (row,col) is valid
bool board_is_full(const Board* board); //Checks if board is finished
void board_clear(Board* board);


#endif
