#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H
#include "board.h"
#include "menu.h"

bool save_game(const GameInfo * gi,const char * filename);
bool load_game(GameInfo * gi,const char * filename);
#endif //SAVE_LOAD_H
