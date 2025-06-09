#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "board.h"
#include <math.h>

#define POP_SIZE 500
#define MAX_GEN 10000
#define TOURNAMENT_K 5
#define MUTATION_RATE 0.1f

#define MAX_SIZE 16

typedef struct {
    int grid[MAX_SIZE][MAX_SIZE];
    int fitness;
} Individual;

static bool fixed[MAX_SIZE][MAX_SIZE];

static int fitness(int grid[MAX_SIZE][MAX_SIZE], int size) {
    int fit = 0;
    for (int c = 0; c < size; ++c) {
        bool seen[MAX_SIZE + 1] = {false};
        for (int r = 0; r < size; ++r) seen[grid[r][c]] = true;
        for (int n = 1; n <= size; ++n) if (seen[n]) fit++;
    }
    int section = (int)sqrt(size);
    for (int br = 0; br < section; ++br) {
        for (int bc = 0; bc < section; ++bc) {
            bool seen[MAX_SIZE + 1] = {false};
            for (int r = br * section; r < br * section + section; ++r)
                for (int c = bc * section; c < bc * section + section; ++c)
                    seen[grid[r][c]] = true;
            for (int n = 1; n <= size; ++n) if (seen[n]) fit++;
        }
    }
    return fit;
}

static void fill_row_random(int row[MAX_SIZE], bool is_fixed[MAX_SIZE], int size) {
    bool present[MAX_SIZE + 1] = {false};
    for (int i = 0; i < size; ++i) if (is_fixed[i]) present[row[i]] = true;
    int missing[MAX_SIZE], m = 0;
    for (int n = 1; n <= size; ++n) if (!present[n]) missing[m++] = n;
    for (int i = m - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int t = missing[i]; missing[i] = missing[j]; missing[j] = t;
    }
    int k = 0;
    for (int i = 0; i < size; ++i) if (!is_fixed[i]) row[i] = missing[k++];
}

static void init_population(Individual pop[POP_SIZE], int base[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < POP_SIZE; ++i) {
        for (int r = 0; r < size; ++r) {
            bool is_fixed[MAX_SIZE];
            for (int c = 0; c < size; ++c) {
                pop[i].grid[r][c] = base[r][c];
                fixed[r][c] = base[r][c] != 0;
                is_fixed[c] = fixed[r][c];
            }
            fill_row_random(pop[i].grid[r], is_fixed, size);
        }
        pop[i].fitness = fitness(pop[i].grid, size);
    }
}

static Individual* tournament(Individual pop[POP_SIZE]) {
    Individual* best = NULL;
    for (int i = 0; i < TOURNAMENT_K; ++i) {
        Individual* cand = &pop[rand() % POP_SIZE];
        if (!best || cand->fitness > best->fitness) best = cand;
    }
    return best;
}

static Individual* roulette(Individual pop[POP_SIZE], int total_fitness) {
    int r = rand() % total_fitness;
    int sum = 0;
    for (int i = 0; i < POP_SIZE; ++i) {
        sum += pop[i].fitness;
        if (sum >= r) return &pop[i];
    }
    return &pop[POP_SIZE - 1];
}

static void uniform_crossover(const Individual* p1, const Individual* p2, Individual* child, int size) {
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (rand() % 2)
                child->grid[r][c] = p1->grid[r][c];
            else
                child->grid[r][c] = p2->grid[r][c];
        }
    }
}

static void crossover(const Individual* p1, const Individual* p2, Individual* child, int size) {
    for (int r = 0; r < size; ++r) {
        const int (*src)[MAX_SIZE] = (rand() & 1) ? p1->grid : p2->grid;
        memcpy(child->grid[r], src[r], sizeof(int) * size);
    }
}

static void mutate(Individual* ind, int size) {
    for (int r = 0; r < size; ++r) {
        if (((float)rand() / RAND_MAX) < MUTATION_RATE) {
            int avail[MAX_SIZE], cnt = 0;
            for (int c = 0; c < size; ++c) if (!fixed[r][c]) avail[cnt++] = c;
            if (cnt >= 2) {
                int i = rand() % cnt, j = rand() % cnt;
                int c1 = avail[i], c2 = avail[j];
                int t = ind->grid[r][c1];
                ind->grid[r][c1] = ind->grid[r][c2];
                ind->grid[r][c2] = t;
            }
        }
    }
}

void ga_solve(Board* board) {
    int size = board->size;
    srand(time(NULL));
    int base[MAX_SIZE][MAX_SIZE];
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            base[r][c] = board_get(board, r, c);

    Individual pop[POP_SIZE], newpop[POP_SIZE];
    init_population(pop, base, size);

    int max_fitness = size * size + ((int)sqrt(size)) * ((int)sqrt(size)) * size;

    for (int gen = 0; gen < MAX_GEN; ++gen) {
        int best_fitness = 0, best_idx = 0;
        for (int i = 0; i < POP_SIZE; ++i) {
            if (pop[i].fitness > best_fitness) {
                best_fitness = pop[i].fitness;
                best_idx = i;
            }
        }
        if (best_fitness == max_fitness ) {
            for (int r = 0; r < size; ++r)
                for (int c = 0; c < size; ++c)
                    board_set(board, r, c, pop[best_idx].grid[r][c]);
            printf("Solved in generation %d!\n", gen);
            return;
        }
        for (int i = 0; i < POP_SIZE; ++i) {
            Individual *p1 = tournament(pop);
            Individual *p2 = tournament(pop);
            //Individual *p1 = roulette(pop, max_fitness);
            //Individual *p2 = roulette(pop, max_fitness);
            //crossover(p1, p2, &newpop[i], size);
            uniform_crossover(p1, p2, &newpop[i], size);
            mutate(&newpop[i], size);
            newpop[i].fitness = fitness(newpop[i].grid, size);
        }
        memcpy(pop, newpop, sizeof(pop));
    }
    int best_idx = 0;
    for (int i = 1; i < POP_SIZE; ++i)
        if (pop[i].fitness > pop[best_idx].fitness) best_idx = i;
    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c)
            board_set(board, r, c, pop[best_idx].grid[r][c]);
    printf("Best solution found after %d generations.\n", MAX_GEN);
}
