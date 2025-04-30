#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

static bool fill_cell(Board * board,int row, int col);

// Helper to shuffle an array in-place
void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

Board* generate_board(int size) {
    if (size<=0) {
        return NULL;
    }
    srand((unsigned)time(NULL));
    Board* b = board_create(size);
    if (!b) {
        printf("board_create failed\n");
        return NULL;
    }
    if (!fill_cell(b,0,0)) {
        printf("fill_cell failed\n");
        board_free(b);
        return NULL;
    }
    return b;
}
static bool fill_cell(Board * board,int row, int col) {
    if (row == board->size) {
        return true; //All rows filled
    }
    int next_row = (col + 1 == board->size) ? row + 1 : row;
    int next_col = (col + 1) % board->size;

    int n = board->size;
    int *vals = malloc(n*sizeof(int));
    if (!vals) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        vals[i] = i+1;
    }
    shuffle(vals, n);

    for (int i = 0; i < n; i++) {
        int val = vals[i];
        if (cell_is_valid(board, row, col, val)) {
            board_set(board, row, col, val);
            if (fill_cell(board, next_row, next_col)) {
                free(vals);
                return true;
            }
            board_set(board, row, col, 0); //Backtracking
        }
    }
    free(vals);
    return false; //no valid value found
}
bool remove_cells(Board* full_board, int k) {
    if (!full_board|| k<= 0 || k > full_board->size*full_board->size) {
        return false;
    }
    int n = full_board->size;
    int attempts = 0;
    int removed = 0;

    while (removed < k && attempts < n*n*5) {
        int row = rand() % n;
        int col = rand() % n;

        int original = board_get(full_board, row, col);
        if (original == 0) {
            attempts++;
            continue;
        }
        board_set(full_board, row, col, 0);
        int solutions = count_solutions(full_board);

        if (solutions == 1) {
            removed++;
        }
        else {
            board_set(full_board, row, col, original);
        }
        attempts++;
    }
    return (removed == k);
}
int solve_board(Board* board,int row, int col, int* count) {
    int n = board->size;
    if (row == n) {
        (*count)++;
        return *count;
    }

    if (*count > 1) return *count;

    int next_row = (col + 1 == board->size) ? row + 1 : row;
    int next_col = (col + 1) % board->size;
    if (board_get(board, row, col) != 0) {
        return solve_board(board, next_row, next_col, count);
    }
    for (int val =1; val <= n; val++) {
        if (cell_is_valid(board, row, col, val)) {
            board_set(board, row, col, val);
            solve_board(board, next_row, next_col, count);
            board_set(board, row, col, 0);
            if (*count > 1) return *count;
        }
    }
    return *count;
}
int count_solutions(Board* board) {
    if (!board) {
        return 0;
    }
    int count = 0;
    solve_board(board,0,0,&count);
    return count;
}

