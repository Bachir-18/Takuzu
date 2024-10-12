#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int size;   // Number of elements in a row
  char *grid; // Pointer to the grid
} t_grid;

void PrintHelp();
void grid_allocate(t_grid *g, int size);
void grid_free(t_grid *g);
void grid_print(t_grid *g, FILE *fd);
bool check_char(const char c);
void file_parser(t_grid *grid, char *filename);

#endif /* UTILITY_H */