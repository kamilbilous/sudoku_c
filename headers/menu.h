#ifndef MENU_H
#define MENU_H

#include "board.h"
#include <time.h>

typedef struct { //Stores game info for saving/loading
    Board* board;
    int size;
    int difficulty;
    time_t start_time;
} GameInfo;
void menu(void);

#endif //MENU_H
