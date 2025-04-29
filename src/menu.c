#include <stdio.h>
#include <stdlib.h>


void menu() {
    int choice;

    while (1) {
        printf("\n    SUDOKU\n");
        printf("1. New Game\n");
        printf("2. Instructions\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                //gamestart
                printf("Placeholder\n");
                return 0;
            case 2:
                printf("\nThe player needs to fill the empty spaces with numbers\n"
                       "so that the same number doesn't appear more than once in:\n"
                       "1. Row\n"
                       "2. Column\n"
                       "3. Outlined square\n"
                       "4. Diagonal (hard mode)\n"
                       "Good luck!\n");
                break;
            case 3:
                printf("Exiting...\n");
                return ;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

