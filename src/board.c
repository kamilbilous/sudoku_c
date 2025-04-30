#include <stdio.h>
#include "board.h"
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

Board* board_create(int size) {
    if (size <= 0) {
        return NULL;
    }
    Board* board = (Board*)malloc(sizeof(Board));
    if (!board) {
        return NULL;
    }
    board->size = size;
    board->cells = calloc(size*size, sizeof(int));
    if (!board->cells) {
        free(board);
        return NULL;
    }
    return board;
}
void board_free(Board* board) {
    if (!board) {
        return;
    }
    free(board->cells);
    free(board);
}
void board_print(Board* board) {
    if (!board) {
        return;
    }
    int n = board->size;
    printf("  ");
    for (int c = 0; c < n; c++) {
        printf("%d |",c+1);
    }
    printf("\n");
    for (int r = 0; r<n; r++) {
        printf("%2d ",r+1);
        for (int c = 0; c<n; c++) {
            int v = board_get(board,r,c);
            if (v>0) {
                printf(" %d",v);
            }
            else {
                printf(" 0");
            }
        }
        printf("\n");
    }

}
bool board_set(Board* board, int row, int col,int val) {
    if (!board || row<0 || col<0 || row >= board->size || col >= board->size) {
        return false;
    }
    board->cells[row*board->size+col] = val;
    return true;
}
int board_get(const Board* board, int row, int col) {
    if (!board || row<0 || col<0 || row >= board->size || col >= board->size) {
        return -1;
    }
    return board->cells[row*board->size+col];
}
bool cell_is_valid(const Board* board, int row, int col, int val) {
    if (!board || val < 1 || val > board->size) {
        return false;
    }
    int n = board->size;
    for (int i =0; i<n; i++) {
        if (board_get(board,i,col) == val) {
            return false;
        }
        if (board_get(board,row,i) == val) {
            return false;
        }
    }
    int section = (int) sqrt(n);
    int start_row = (row/section)*section;
    int start_col = (col/section)*section;
    for (int i =0; i<section; i++) {
        for (int j =0; j<section; j++) {
            if (board_get(board,start_row+i,start_col+j) == val) {
                return false;
            }
        }
    }
    return true;
}
bool board_is_full(const Board* board) {
    if (!board) {
        return false;
    }
    int total_size = board->size * board->size;
    for (int i =0; i<total_size; i++) {
        if (board->cells[i] == 0) {
            return false;
        }
    }
    return true;
}
void board_clear(Board* board) {
    if (!board) return;
    memset(board->cells, 0, board->size * board->size * sizeof(int));
}
