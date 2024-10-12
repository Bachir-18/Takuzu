#include "../include/grid.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void grid_copy(t_grid *gs, t_grid *gd) {

  grid_allocate(gd, gs->size); // we suppose that gd it not intialised yet
  int loopsize = gs->size * gs->size;
  // Perform a copy of the grid data
  for (int i = 0; i < loopsize; i++) {
    gd->grid[i] = gs->grid[i];
  }
}

void set_cell(int i, int j, t_grid *g, char v) {

  // i= lines j = col

  // Check if the given indices are within the grid bounds
  if (i >= 0 && i < g->size && j >= 0 && j < g->size) {
    // Check if the provided character is valid
    if (check_char(v)) {
      int index = i * g->size + j;
      g->grid[index] = v;
    } else {
      fprintf(stderr, "Error: Invalid character '%c'\n", v);
      grid_free(g);
      exit(EXIT_FAILURE);
    }
  } else {
    fprintf(stderr, "Error: Invalid cell indices (%d, %d)\n", i, j);
    grid_free(g);
    exit(EXIT_FAILURE);
  }
}

char get_cell(int i, int j, t_grid *g) {
  // Check if the given indices are within the grid bounds
  if (i >= 0 && i < g->size && j >= 0 && j < g->size) {
    int index = i * g->size + j;
    return g->grid[index];
  } else {
    fprintf(stderr, "Error: Invalid c ell indices (%d, %d)\n", i, j);
    grid_free(g);
    exit(EXIT_FAILURE);
  }
}

static bool checkLinesCol(t_grid *g) {
  for (int i = 0; i < g->size; i++) {
    for (int j = i + 1; j < g->size; j++) {
      bool identicalRows = true;
      bool identicalCols = true;
      bool hasNonEmptyCell = false;

      // Check for identical rows with non-empty cells
      for (int k = 0; k < g->size; k++) {
        char cell1 = get_cell(i, k, g);
        char cell2 = get_cell(j, k, g);

        if (cell1 == '_' || cell2 == '_') {
          // Skip checking if any cell is empty
          identicalRows = false;
          break;
        }

        hasNonEmptyCell = true;

        if (cell1 != cell2) {
          identicalRows = false;
          break;
        }
      }

      // Check for identical columns with non-empty cells
      for (int k = 0; k < g->size; k++) {
        char cell1 = get_cell(k, i, g);
        char cell2 = get_cell(k, j, g);

        if (cell1 == '_' || cell2 == '_') {
          // Skip checking if any cell is empty
          identicalCols = false;
          break;
        }

        if (cell1 != cell2) {
          identicalCols = false;
          break;
        }
      }

      if (hasNonEmptyCell && (identicalRows || identicalCols)) {
        return true; // Found identical lines or columns, return true
      }
    }
  }

  return false; // No identical lines or columns found
}
static bool is_full(t_grid *g) {
  // Check if the grid is full (no empty cells)
  for (int i = 0; i < g->size; i++) {
    for (int j = 0; j < g->size; j++) {
      if (get_cell(i, j, g) == '_') {
        return false;
      }
    }
  }
  return true;
}
static bool equalRowLines(t_grid *g) {
  // Check if the grid has an equal number of 1s and 0s in each row and column
  // row
  for (int i = 0; i < g->size; i++) {
    int nb1 = 0, nb0 = 0;
    for (int j = 0; j < g->size; j++) {

      if (get_cell(i, j, g) != '0') {
        nb0++;
      }

      if (get_cell(i, j, g) != '1') {
        nb1++;
      }
    }

    if (nb0 != nb1) {
      return false;
    }
  }

  // column

  for (int i = 0; i < g->size; i++) {
    int nb1 = 0, nb0 = 0;
    for (int j = 0; j < g->size; j++) {

      if (get_cell(j, i, g) != '0') {
        nb0++;
      }

      if (get_cell(j, i, g) != '1') {
        nb1++;
      }
    }

    if (nb0 != nb1) {
      return false;
    }
  }
  return true;
}

bool is_consistent(t_grid *g, int verbose) {
  if (checkLinesCol(g)) {
    if (verbose) {
      fprintf(stdout,
              "######################################################\n");
      fprintf(stdout, "Two identical (rows | columns) found in this grid\n");
    }
    return false;
  }

  if (g->grid == NULL) {
    return false;
  }

  // Check for more than two consecutive zeros or ones in rows and columns
  for (int i = 0; i < g->size; i++) {
    for (int j = 0; j < g->size; j++) {
      // Check rows
      if (get_cell(i, j, g) != '_') {
        int k = j + 1;
        int l = k + 1;
        if (l < g->size && (get_cell(i, j, g) == get_cell(i, k, g)) &&
            get_cell(i, k, g) == get_cell(i, l, g)) {
          if (verbose) {
            fprintf(stdout,
                    "######################################################\n");
            fprintf(stdout,
                    "More than two consecutives %c found in the row %d\n",
                    get_cell(i, j, g), i + 1);
          }
          return false;
        }
      }

      // Check columns
      if (get_cell(j, i, g) != '_') {
        int k = j + 1;
        int l = k + 1;
        if (l < g->size && (get_cell(j, i, g) == get_cell(k, i, g)) &&
            get_cell(k, i, g) == get_cell(l, i, g)) {
          if (verbose) {
            fprintf(stdout,
                    "######################################################\n");
            fprintf(stdout,
                    "More than two consecutives %c found in the row %d\n",
                    get_cell(k, i, g), i + 1);
          }
          return false;
        }
      }
    }
  }
  if (is_full(g) && !equalRowLines(g)) {
    return false;
  }
  // The grid is consistent
  return true;
}

bool is_valid(t_grid *g) {
  // Check if the grid is full (no empty cells)
  for (int i = 0; i < g->size; i++) {
    for (int j = 0; j < g->size; j++) {
      if (get_cell(i, j, g) == '_') {
        return false;
      }
    }
  }

  // Check if the grid is consistent
  if (!is_consistent(g, 0)) {
    return false;
  }

  if (!equalRowLines(g)) {
    return false;
  }

  // The grid is valid
  return true;
}

bool one_possible_value_heuristic(t_grid *g) {
  bool is_modified = false;

  // Check each row for single candidate
  for (int i = 0; i < g->size; i++) {
    int empty_cell_count = 0;
    int empty_cell_index = -1;
    int zero_count = 0;
    int one_count = 0;

    // Count the number of empty cells and find the index of the first empty
    // cell
    for (int j = 0; j < g->size; j++) {
      if (get_cell(i, j, g) == '_') {
        empty_cell_count++;
        empty_cell_index = j;
      } else if (get_cell(i, j, g) == '0') {
        zero_count++;
      } else if (get_cell(i, j, g) == '1') {
        one_count++;
      }
    }

    // If there is only one empty cell and the counts of zeros and ones are not
    // equal
    if (empty_cell_count == 1 && zero_count != one_count) {
      // Fill the empty cell with the opposite value
      char opposite_value = (zero_count > one_count) ? '1' : '0';
      set_cell(i, empty_cell_index, g, opposite_value);
      is_modified = true;
    }
  }

  // Check each column for single candidate
  for (int j = 0; j < g->size; j++) {
    int empty_cell_count = 0;
    int empty_cell_index = -1;
    int zero_count = 0;
    int one_count = 0;

    // Count the number of empty cells and find the index of the first empty
    // cell
    for (int i = 0; i < g->size; i++) {
      if (get_cell(i, j, g) == '_') {
        empty_cell_count++;
        empty_cell_index = i;
      } else if (get_cell(i, j, g) == '0') {
        zero_count++;
      } else if (get_cell(i, j, g) == '1') {
        one_count++;
      }
    }

    // If there is only one empty cell and the counts of zeros and ones are
    // notequal
    if (empty_cell_count == 1 && zero_count != one_count) {
      // Fill the empty cell with the opposite value
      char opposite_value = (zero_count > one_count) ? '1' : '0';
      set_cell(empty_cell_index, j, g, opposite_value);
      is_modified = true;
    }
  }

  return is_modified;
}

bool check_consecutive_heuristic(t_grid *g) {
  bool is_modified = false;
  // Check rows
  for (int i = 0; i < g->size; i++) {
    for (int j = 0; j < g->size - 2; j++) {
      if (get_cell(i, j, g) == '0' && get_cell(i, j + 1, g) == '0') {
        if (get_cell(i, j + 2, g) == '_') {
          set_cell(i, j + 2, g, '1');
          is_modified = true;
        }
        if (j > 0) {
          if (get_cell(i, j - 1, g) == '_') {
            set_cell(i, j - 1, g, '1');
            is_modified = true;
          }
        }

      } else if (get_cell(i, j, g) == '1' && get_cell(i, j + 1, g) == '1') {
        if (get_cell(i, j + 2, g) == '_') {
          set_cell(i, j + 2, g, '0');
          is_modified = true;
        }
        if (j > 0) {
          if (get_cell(i, j - 1, g) == '_') {
            set_cell(i, j - 1, g, '0');
            is_modified = true;
          }
        }
      }
    }
    // Manage two last cells of a given row
    if (get_cell(i, g->size - 2, g) == '1' &&
        get_cell(i, g->size - 1, g) == '1') {
      if (get_cell(i, g->size - 3, g) == '_') {
        set_cell(i, g->size - 3, g, '0');
        is_modified = true;
      }
    }
    if (get_cell(i, g->size - 2, g) == '0' &&
        get_cell(i, g->size - 1, g) == '0') {
      if (get_cell(i, g->size - 3, g) == '_') {
        set_cell(i, g->size - 3, g, '1');
        is_modified = true;
      }
    }
  }

  // Check columns
  for (int j = 0; j < g->size; j++) {
    for (int i = 0; i < g->size - 2; i++) {
      if (get_cell(i, j, g) == '0' && get_cell(i + 1, j, g) == '0') {
        if (get_cell(i + 2, j, g) == '_') {
          set_cell(i + 2, j, g, '1');
          is_modified = true;
        }
        if (i > 0) {
          if (get_cell(i - 1, j, g) == '_') {
            set_cell(i - 1, j, g, '1');
            is_modified = true;
          }
        }
      } else if (get_cell(i, j, g) == '1' && get_cell(i + 1, j, g) == '1') {
        if (get_cell(i + 2, j, g) == '_') {
          set_cell(i + 2, j, g, '0');
          is_modified = true;
        }
        if (i > 0) {
          if (get_cell(i - 1, j, g) == '_') {
            set_cell(i - 1, j, g, '0');
            is_modified = true;
          }
        }
      }
    }
    // Manage two last cells of a given column
    if (get_cell(g->size - 2, j, g) == '1' &&
        get_cell(g->size - 1, j, g) == '1') {
      if (get_cell(g->size - 3, j, g) == '_') {
        set_cell(g->size - 3, j, g, '0');
        is_modified = true;
      }
    }
    if (get_cell(g->size - 2, j, g) == '0' &&
        get_cell(g->size - 1, j, g) == '0') {
      if (get_cell(g->size - 3, j, g) == '_') {
        set_cell(g->size - 3, j, g, '1');
        is_modified = true;
      }
    }
  }

  return is_modified;
}

bool filled_empty_cell_heuristic(t_grid *g) {
  bool is_modified = false;

  // Check rows
  for (int i = 0; i < g->size; i++) {
    int nbZeros = 0;
    int nbOnes = 0;
    int nbEmpty = 0;

    for (int j = 0; j < g->size; j++) {
      char cell = get_cell(i, j, g);
      if (cell == '0') {
        nbZeros++;
      } else if (cell == '1') {
        nbOnes++;
      } else if (cell == '_') {
        nbEmpty++;
      }
    }

    // if (nbZeros == g->size - nbEmpty)
    if (nbZeros == g->size / 2) {
      for (int j = 0; j < g->size; j++) {
        if (get_cell(i, j, g) == '_') {
          set_cell(i, j, g, '1');
          is_modified = true;
        }
      }
    } else if (nbOnes == g->size / 2) {
      for (int j = 0; j < g->size; j++) {
        if (get_cell(i, j, g) == '_') {
          set_cell(i, j, g, '0');
          is_modified = true;
        }
      }
    }
  }

  // Check columns
  for (int j = 0; j < g->size; j++) {
    int nbZeros = 0;
    int nbOnes = 0;
    int nbEmpty = 0;

    for (int i = 0; i < g->size; i++) {
      char cell = get_cell(i, j, g);
      if (cell == '0') {
        nbZeros++;
      } else if (cell == '1') {
        nbOnes++;
      } else if (cell == '_') {
        nbEmpty++;
      }
    }

    if (nbZeros == g->size / 2) {
      for (int i = 0; i < g->size; i++) {
        if (get_cell(i, j, g) == '_') {
          set_cell(i, j, g, '1');
          is_modified = true;
        }
      }
    } else if (nbOnes == g->size / 2) {
      for (int i = 0; i < g->size; i++) {
        if (get_cell(i, j, g) == '_') {
          set_cell(i, j, g, '0');
          is_modified = true;
        }
      }
    }
  }

  return is_modified;
}

void stabilise_with_heuristics(t_grid *grid) {

  while (check_consecutive_heuristic(grid) ||
         filled_empty_cell_heuristic(grid) ||
         one_possible_value_heuristic(grid)) {
    ;
  }
}

void grid_choice_apply(t_grid *grid, const choice_t choice) {
  set_cell(choice.row, choice.column, grid, choice.choice);
}

void grid_choice_print(const choice_t choice, FILE *fd) {
  fprintf(fd, "Choice made: ");
  fprintf(fd, "Row = %d, Column = %d, Choice = %c\n", choice.row + 1,
          choice.column + 1, choice.choice);
}

choice_t grid_choice(t_grid *grid) {
  choice_t choice;
  choice.row = -1;
  choice.column = -1;
  choice.choice = '_';

  // Iterate through each cell in the grid
  for (int i = 0; i < grid->size; i++) {
    for (int j = 0; j < grid->size; j++) {
      // Check if the cell is empty
      if (get_cell(i, j, grid) == '_') {
        // Set the choice to '0' and check if it makes the grid inconsistent
        set_cell(i, j, grid, '0');
        if (is_consistent(grid, 0)) {
          choice.row = i;
          choice.column = j;
          choice.choice = '0';
          return choice;
        }

        // Set the choice to '1' and check if it makes the grid inconsistent
        set_cell(i, j, grid, '1');
        if (is_consistent(grid, 0)) {
          choice.row = i;
          choice.column = j;
          choice.choice = '1';
          return choice;
        }

        // Reset the cell back to empty
        // set_cell(i, j, grid, '_');
      }
    }
  }

  return choice;
}

static choice_t secondChoice(choice_t choice) {
  choice_t other_choice;
  other_choice = choice;
  switch (choice.choice) {
  case '0':
    other_choice.choice = '1';
    break;
  case '1':
    other_choice.choice = '0';
  }
  return other_choice;
}
// Search for all possible solutions and return the grid found
static t_grid *seek_for_solution(t_grid *grid, int *nb, int verbose) {
  if (*nb == 1) {
    return grid;
  }

  if (is_valid(grid)) {
    *nb += 1;
    return grid;
  }

  if (!is_consistent(grid, verbose)) {
    if (verbose) {
      printf("Inconsistent grid!, exploring other remaining paths...\n");
      grid_print(grid, stdout);
    }
    grid_free(grid);
    return NULL;
  }

  choice_t choice;
  choice_t otherChoice;
  t_grid gridCopy1;
  t_grid gridCopy2;
  grid_copy(grid, &gridCopy1);
  choice = grid_choice(&gridCopy1);
  // If choice.row == -1, it means (according to our implementation) that no
  // choice will lead to a consistent grid therefore the grid is inconstent
  if (choice.row == -1) {
    return false;
  }
  grid_copy(&gridCopy1, &gridCopy2);
  grid_choice_apply(&gridCopy1, choice);
  // Applying heuristics
  stabilise_with_heuristics(&gridCopy1);

  if (is_consistent(&gridCopy1, 0)) {
    t_grid *result = seek_for_solution(&gridCopy1, nb, verbose);
    if (result != NULL) {
      return result;
    }
  }

  otherChoice = secondChoice(choice);

  set_cell(otherChoice.row, otherChoice.column, &gridCopy2, '_');
  grid_choice_apply(&gridCopy2, otherChoice);
  stabilise_with_heuristics(&gridCopy2);

  return seek_for_solution(&gridCopy2, nb, verbose);
}
// Check if the grid has only one solution or not in order to generate a grid
// with one solution
static bool is_there_any_other_solution(t_grid *grid, int *nb, int verbose) {

  if (is_valid(grid)) {
    *nb += 1;
    if (verbose) {
      printf("Consistent grid!, Number of solutions found %d\n", *nb);
      grid_print(grid, stdout);
    }
    grid_free(grid);
    return true;
  }

  if (!is_consistent(grid, verbose)) {
    if (verbose) {
      printf("Inconsistent grid!, exploring other remaining paths...\n");
      grid_print(grid, stdout);
    }
    grid_free(grid);
    return false;
  }

  choice_t choice;
  choice_t otherChoice;
  t_grid gridCopy1;
  t_grid gridCopy2;

  grid_copy(grid, &gridCopy1);
  grid_copy(grid, &gridCopy2);
  choice = grid_choice(&gridCopy1);
  // If choice.row == -1, it means (according to our implementation) that no
  // choice will lead to a consistent grid therefore the grid is inconstent
  if (choice.row == -1) {
    return false;
  }
  // Save the second choice in case we want to do backtracking
  otherChoice = secondChoice(choice);
  grid_choice_apply(&gridCopy1, choice);

  if (is_consistent(&gridCopy1, 0)) {
    stabilise_with_heuristics(&gridCopy1);
    is_there_any_other_solution(&gridCopy1, nb, verbose);
  }

  set_cell(otherChoice.row, otherChoice.column, &gridCopy2, '_');

  grid_choice_apply(&gridCopy2, otherChoice);

  if (is_consistent(&gridCopy2, 0)) {
    stabilise_with_heuristics(&gridCopy2);
    is_there_any_other_solution(&gridCopy2, nb, verbose);
  }

  return false;
}
// Search for a grid which have only one solution
t_grid *generateUniqueSolution(t_grid *grid, int verbose) {

  srand(time(NULL));
  int nb = 0;
  t_grid grid2;
  // Search a grid having at least one solution
  grid_copy(seek_for_solution(grid, &nb, verbose), &grid2);
  grid_free(grid);
  int size = grid2.size;
  // Seach for grid having having only one solution
  while (nb < 2) {
    nb = 0;
    int i, j;

    i = rand() % size;
    j = rand() % size;

    while (get_cell(i, j, &grid2) == '_') {
      i = rand() % size;
      j = rand() % size;
    }
    char tmp = get_cell(i, j, &grid2);

    set_cell(i, j, &grid2, '_');

    t_grid gridCopy;
    grid_copy(&grid2, &gridCopy);
    is_there_any_other_solution(&gridCopy, &nb, verbose);
    // If nb == 1 this means that the number of solution is 1, we found what we
    // searched for
    if (nb == 1) {

      ;
    } else {
      // else we change the value of the cell and we continue searching
      set_cell(i, j, &grid2, tmp);
      grid_copy(&grid2, grid);

      return grid;
    }
  }

  return NULL;
}

static bool has_solution(t_grid *grid, FILE *output, int verbose, int unique) {
  if (is_valid(grid)) {
    if (!unique) {
      fprintf(output,
              "######################################################\n");
      fprintf(output, "Solution found\n");
      grid_print(grid, output);
      fprintf(output, "\n\n");
    }
    grid_free(grid);
    // exit(EXIT_SUCCESS);
    return true;
  }

  if (!is_consistent(grid, verbose) || is_full(grid)) {
    if (verbose) {
      printf("######################################################\n");
      printf("Inconsistent grid!, exploring other remaining paths...\n");
      grid_print(grid, stdout);
    }
    grid_free(grid);
    return false;
  }

  // Other make a choice and continue exploring
  choice_t choice;
  choice_t other_choice;
  t_grid gridCopy1;
  t_grid gridCopy2;
  grid_copy(grid, &gridCopy1); // gridCopy1 is used to explore one path
  grid_copy(grid, &gridCopy2); // gridCopy2 is used to explore the remaining
  choice = grid_choice(&gridCopy1);
  // If choice.row == -1, it means (according to our implementation) that no
  // choice will lead to a consistent grid therefore the grid is inconstent
  if (choice.row == -1) {
    return false;
  }
  // Save the second choice in case we want to do backtracking
  other_choice = secondChoice(choice);

  if (verbose) {
    printf("######################################################\n");
    printf("Choice made....\n");
    grid_choice_print(choice, stdout);
  }
  grid_choice_apply(&gridCopy1, choice);
  // Applying heuristics
  if (is_consistent(&gridCopy1, 0)) {
    stabilise_with_heuristics(&gridCopy1);
  }

  if (verbose) {
    printf("Result of the exploration!\n");
    grid_print(&gridCopy1, stdout);
  }

  if (is_consistent(&gridCopy1, verbose)) {
    // if this path leads to a solution, we don't need to do backtracking
    if (has_solution(&gridCopy1, output, verbose, unique)) {
      return true;
    }
  }
  // if the firs path  doesn't lead to a solution, we do backtracking
  if (verbose) {
    printf("######################################################\n");
    printf("Inconsitent path, bactracking...!\n");
    printf("Choice made....\n");
    grid_choice_print(other_choice, stdout);
    grid_print(&gridCopy2, stdout);
  }

  set_cell(other_choice.row, other_choice.column, &gridCopy2, '_');
  grid_choice_apply(&gridCopy2, other_choice);
  if (is_consistent(&gridCopy2, 0)) {
    stabilise_with_heuristics(&gridCopy2);
  }

  return has_solution(&gridCopy2, output, verbose, unique);
}

static void grid_constructor(int size, t_grid *g, int N) {
  srand(time(NULL));
  // Calculate the number of cells to be filled with '0' and '1'
  int num_cells = size * size;
  int num_filled_cells = (N * num_cells) / 100;

  //  allocate the grid and Initialize it with empty cells
  grid_allocate(g, size);
  for (int i = 0; i < num_cells; i++) {
    g->grid[i] = '_';
  }
  // Fill the grid randomly with '0' and check consitency
  for (int i = 0; i < abs(num_filled_cells / 2); i++) {
    int line, column;
    int row, col;
    do {
      line = rand() % size;
      column = rand() % size;
      while (line == row && column == col) {
        line = rand() % size;
        column = rand() % size;
      }

    } while (get_cell(line, column, g) != '_');

    set_cell(line, column, g, '0');

    if (!is_consistent(g, 0)) {
      set_cell(line, column, g, '_');
      i--;
      row = line;
      col = column;
    }
  }
  // Fill the grid randomly with '1' and check consitency
  for (int i = 0; i < abs(num_filled_cells / 2); i++) {
    int line, column;
    int row, col;
    do {
      line = rand() % size;
      column = rand() % size;
      while (line == row && column == col) {
        line = rand() % size;
        column = rand() % size;
      }
    } while (get_cell(line, column, g) != '_');

    set_cell(line, column, g, '1');
    if (!is_consistent(g, 0)) {
      set_cell(line, column, g, '_');

      i--;
      row = line;
      col = column;
    }
  }
}

// Search and return all solutions if there is
static bool search_solutions(t_grid *grid, FILE *output, int verbose, int *nb) {
  // If the grid is valid it means that a solution is found
  if (is_valid(grid)) {
    *nb += 1;
    fprintf(output, "######################################################\n");
    fprintf(output, "Solution n° %d\n", *nb);
    grid_print(grid, output);
    fprintf(output, "\n\n");
    grid_free(grid);
    return true;
  }
  // if the given grid is inconsistent then return false
  if (!is_consistent(grid, verbose)) {
    if (verbose) {
      printf("Inconsistent grid!, exploring other remaining paths...\n");
      grid_print(grid, stdout);
    }
    grid_free(grid);
    return false;
  }
  // Other make a choice and continue exploring
  choice_t choice;
  choice_t other_choice;
  t_grid gridCopy1;
  t_grid gridCopy2;
  grid_copy(grid, &gridCopy1); // gridCopy1 is used to explore one path
  grid_copy(grid, &gridCopy2); // gridCopy2 is used to explore the remaining
                               // path
  choice = grid_choice(&gridCopy1);
  // If choice.row == -1, it means (according to our implementation) that no
  // choice will lead to a consistent grid therefore the grid is inconstent
  if (choice.row == -1) {
    return false;
  }
  // We store the secondChoice to explore remaining paths
  other_choice = secondChoice(choice);
  grid_choice_apply(&gridCopy1, choice);
  // Checking consistency after applying the choice
  if (is_consistent(&gridCopy1, 0)) {
    // Apply heuristics to the grid
    stabilise_with_heuristics(&gridCopy1);
    if (verbose) {
      printf("######################################################\n");
      printf("First choice.... \n");
      grid_choice_print(choice, stdout);
      printf("Result of the exploration!\n");
      grid_print(&gridCopy1, stdout);
    }
    search_solutions(&gridCopy1, output, verbose, nb);
  }
  // Explore the remaining path as we search all solution
  set_cell(other_choice.row, other_choice.column, &gridCopy2, '_');
  grid_choice_apply(&gridCopy2, other_choice);
  if (is_consistent(&gridCopy2, 0)) {
    stabilise_with_heuristics(&gridCopy2);
    if (verbose) {
      printf("######################################################\n");
      printf("Second choice.... \n");
      grid_choice_print(other_choice, stdout);
      printf("Result of the exploration!\n");
      grid_print(&gridCopy2, stdout);
    }
    search_solutions(&gridCopy2, output, verbose, nb);
  }
  // if any path doesn't lead to a solution, the grid is inconsistence
  return false;
}

void generate_grid(int size, int N, t_grid *g, FILE *fd, int unique_mode,
                   int verbose) {
  if (!unique_mode) {
    grid_constructor(size, g, N);
    while (!has_solution(g, fd, verbose, 1)) {
      grid_free(g);
      grid_constructor(size, g, N);
    }
  } else {
    srand(time(NULL));
    // Calculate the number of cells to be filled with '0' and '1'
    int num_cells = size * size;

    //  allocate the grid and Initialize it with empty cells
    grid_allocate(g, size);
    for (int i = 0; i < num_cells; i++) {
      g->grid[i] = '_';
    }
    generateUniqueSolution(g, verbose);
  }

  // if mode unique activated search for a grid having at least one solution
}

t_grid *grid_solver(t_grid *grid, const t_mode mode, FILE *output,
                    int verbose) {

  if (mode == MODE_FIRST) {
    int unique_mode = 0;
    has_solution(grid, output, verbose, unique_mode);
  } else if (mode == MODE_ALL) {
    int nb_solutions = 0;
    fprintf(output, "Searching for all solutions...\n");
    search_solutions(grid, output, verbose, &nb_solutions);
    fprintf(output, "######################################################\n");
    fprintf(output, "Number of solutions found %d\n", nb_solutions);
    fprintf(output, "######################################################\n");
  }
  return grid;
}
