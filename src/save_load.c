#include "save_load.h"
#include "board.h"
#include <stdio.h>

bool save_game(const GameInfo * gi, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open save file");
        return false;
    }

    fwrite(&gi->size, sizeof(int), 1, file);
    fwrite(&gi->difficulty, sizeof(int), 1, file);
    fwrite(&gi->start_time, sizeof(time_t), 1, file);
    fwrite(gi->board->cells, sizeof(int), gi->size * gi->size, file);

    fclose(file);
    printf("Game saved to '%s'\n", filename);
    return true;
}
bool load_game(GameInfo * gi, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }
    fread(&gi->size, sizeof(int), 1, file);
    fread(&gi->difficulty, sizeof(int), 1, file);
    fread(&gi->start_time, sizeof(time_t), 1, file);

    Board * board = board_create(gi->size);
    if (!board) {
        fclose(file);
        return false;
    }
    fread(board->cells, sizeof(int), gi->size * gi->size, file);
    gi->board = board;
    fclose(file);

    printf("Game loaded from '%s'\n", filename);
    return true;

}
