#ifndef GRID_H
#define GRID_H
#include "utility.h"
#include <stdio.h>
#include <time.h>

typedef struct {
  int row;
  int column;
  char choice;
} choice_t;
typedef enum { MODE_FIRST, MODE_ALL } t_mode;
void grid_copy(t_grid *gs, t_grid *gd);
void set_cell(int i, int j, t_grid *g, char v);
char get_cell(int i, int j, t_grid *g);
bool is_consistent(t_grid *g, int verbose);
bool is_valid(t_grid *g);
void generate_grid(int size, int N, t_grid *g, FILE *fd, int unique_mode,
                   int verbose);
bool check_consecutive_heuristic(t_grid *g);
bool filled_cell_heuristic(t_grid *g);
void stabilise_with_heuristics(t_grid *grid);
void grid_choice_apply(t_grid *grid, const choice_t choice);
void grid_choice_print(const choice_t choice, FILE *fd);
choice_t grid_choice(t_grid *grid);
t_grid *grid_solver(t_grid *grid, const t_mode mode, FILE *output, int verbose);

#endif
