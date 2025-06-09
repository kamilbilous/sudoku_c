#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "board.h"
#include "generator.h"
#include "save_load.h"
#include "ga_solver.h"


void move_handler(GameInfo* gi) {
    int row, col, val;
    char input[20];
    char row_str[5], col_str[5];
    Board * board = gi->board;
    while (!board_is_full(board)) {
        char val_str[5];
        board_print(board);

        printf("Enter your move (row col value),'s' to save,'g' to solve using genetic alg or 'q' to quit:\n");
        fgets(input, sizeof(input), stdin);
        if (input[0] == 'q' || input[0] == 'Q') {
            printf("Quitting...\n");
            return;
        }
        if (input[0] == 's' || input[0] == 'S') {
            save_game(gi,"save.dat");
            continue;
        }
        if (input[0] == 'g' || input[0] == 'G') {
            ga_solve(board);
            board_print(board);
            return;
        }

        if (sscanf(input, "%s %s %s", row_str, col_str, val_str) != 3) {
            printf("Invalid input. Use: row col value (e.g., 2 3 5 or B F A)\n");
            continue;
        }

        // Convert row
        if (row_str[0] >= 'A' && row_str[0] <= 'Z' || row_str[0] >= 'a' && row_str[0] <= 'z')
            row = 10 + (row_str[0] - 'A');
        else
            row = atoi(row_str) - 1;

        // Convert col
        if (col_str[0] >= 'A' && col_str[0] <= 'Z' || col_str[0] >= 'a' && col_str[0] <= 'z')
            col = 10 + (col_str[0] - 'A');
        else
            col = atoi(col_str) - 1;

        // Convert value from string to int
        if (val_str[0] == '0') {
            val = 0;
        } else if (val_str[0] >= '1' && val_str[0] <= '9') {
            val = val_str[0] - '0';
        } else if (val_str[0] >= 'A' && val_str[0] <= 'Z') {
            val = 10 + (val_str[0] - 'A');
        } else if (val_str[0] >= 'a' && val_str[0] <= 'z') {
            val = 10 + (val_str[0] - 'a');
        } else {
            printf("Invalid value. Use 1-9, A-Z, or 0 to erase.\n");
            continue;
        }

        // Bounds check
        if (row < 0 || col < 0 || row >= board->size || col >= board->size) {
            printf("Out of bounds.\n");
            continue;
        }

        // Check validity (skip if erasing)
        if (val != 0 && !cell_is_valid(board, row, col, val)) {
            printf("Invalid move.\n");
            continue;
        }

        board_set(board, row, col, val);
    }
    time_t end_time = time(NULL); //Stops the times
    int duration = (int)difftime(end_time, gi->start_time);

    printf("\nYou finished in %d : %d.\n",duration/60,duration % 60);
}

static void new_game(GameInfo * gi) {
    printf("Please enter the size of your board:\n"
           "1. 4x4\n"
           "2. 9x9\n"
           "3. 16x16\n");
    scanf("%d",&gi->size);
    printf("\n");
    switch (gi->size) {
        case 1:
            gi->size = 4;
            break;
        case 2:
            gi->size = 9;
            break;
        case 3:
            gi->size = 16;
            break;
        default:
            printf("Incorrect size. Defaulting to 9x9 \n");
            gi->size = 9;
    }


    printf("Choose the difficulty \n"
           "1. Easy (20%% are empty)\n"
           "2. Medium (40%% are empty)\n"
           "3. Hard (60%% are empty)\n");
    scanf("%d", &gi->difficulty);
    printf("\n");

    int empty_cells = 0;
    int total_cells = gi->size * gi->size;
    switch (gi->difficulty) {
        case 1:
            empty_cells = (int)total_cells*0.2;
            break;
        case 2:
            empty_cells = (int)total_cells*0.4;
            break;
        case 3:
            empty_cells = (int)total_cells*0.6;
            break;
        default:
            printf("Incorrect difficulty. Defaulting to Easy \n");
            empty_cells = (int)total_cells*0.2;
    }
    Board * board = generate_board(gi->size);
    if (!board) {
        printf("Failed to generate board\n");
        return;
    }
    if (!remove_cells(board,empty_cells)) {
        printf("Failed to remove cells\n");
        board_free(board);
        return;
    }
    gi->board = board;
    gi->start_time = time(NULL);
    move_handler(gi);

    board_free(board);
}



void menu() {
    int choice;
    GameInfo gi = {0};
    while (1) {
        printf("\n    SUDOKU\n");
        printf("1. New Game\n");
        printf("2. Load game \n");
        printf("3. Instructions\n");
        printf("4. Exit\n");
        scanf("%d", &choice);
        getchar(); //Clean newline
        switch (choice) {
            case 1:
                if (gi.board) {
                    board_free(gi.board);  // Free previous board if any
                    gi.board = NULL;
                }
                new_game(&gi);
                return;
            case 2:
                if (gi.board) {
                    board_free(gi.board);  // Free previous board if any
                    gi.board = NULL;
                }
                if (load_game(&gi, "save.dat")) {
                    move_handler(&gi);
                    board_free(gi.board);
                    gi.board = NULL;
                }
                break;
            case 3:
                printf("\nThe player needs to fill the empty spaces with numbers\n"
                       "so that the same number doesn't appear more than once in:\n"
                       "1. Row\n"
                       "2. Column\n"
                       "3. Outlined square\n"
                       "4. Diagonal (hard mode)\n"
                       "Good luck!\n");
                break;
            case 4:
                printf("Exiting...\n");
                return ;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
