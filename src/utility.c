#include "../include/utility.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void PrintHelp() {

  printf("Usage: takuzu [-a|-o FILE|-v|-h] FILE...\n"
         "takuzu -g[SIZE] [-u|-o FILE|-v|-h]\n"
         "Solve or generate takuzu grids of size: 4, 8, 16, 32, 64\n"
         "-a, --all\tsearch for all possible solutions\n"
         "-g[N], --generate[=N]\tgenerate a grid of size NxN (default: 8)\n"
         "-o FILE, --output FILE\twrite output to FILE\n"
         "-u, --unique\tgenerate a grid with a unique solution\n"
         "-v, --verbose\tverbose output\n"
         "-h, --help\tdisplay this help and exit\n");
  exit(EXIT_SUCCESS);
};

void grid_allocate(t_grid *g, int size) {

  g->size = size;
  g->grid = (char *)malloc(size * size * sizeof(char));

  if (g->grid == NULL) {
    fprintf(stderr, "Error: Memory allocation failed for the grid.\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < (size * size); i++) {

    g->grid[i] = '_';
  }
}

void grid_free(t_grid *g) {

  free(g->grid);
  g->grid = NULL;
  g->size = 0;
}

bool check_char(const char c) { return (c == '0') || (c == '1') || (c == '_'); }

void grid_print(t_grid *g, FILE *fd) {

  int n = g->size;
  for (int i = 0; i < (g->size * g->size); i++) {
    if (n == 0) {
      fprintf(fd, "\n");
      n = g->size;
    }
    fprintf(fd, "%c ", g->grid[i]);
    n--;
  }
  fprintf(fd, "\n");
}

static int get_size(FILE *file, char *filename) {
  int size = 0;
  int line = 1;
  char ch;
  int isFirstChar = 1; // Flag to track the first character of the line (when #
  // is first char of the line)

  while ((ch = fgetc(file)) != EOF) {
    if (isFirstChar && ch == '#') {
      while ((ch = fgetc(file)) != EOF && ch != '\n') {
        ; // do nothing
      }

      line++;

      isFirstChar = 1; // Reset the flag for the next line
      continue;
    }

    else if ((!isFirstChar) && ch == '#') {
      break;
    }
    if (ch == ' ' || ch == '\t' || ch == '\n') {

      continue;

    }

    else if (check_char(ch)) {
      size++;
      isFirstChar = 0; // Not the first character anymore
      ch = fgetc(file);

      if (ch == ' ' || ch == '\t') {

        while ((ch = fgetc(file)) == ' ' || ch == '\t')
          ;
      }

      if (ch == '\n' || ch == '#') {

        break;
      } else {
        ungetc(ch, file); // Put back non-newline char
      }

    } else {
      fprintf(stderr, "Error: wrong character '%c' at line %d of the file!\n",
              ch, line);
      if (fclose(file) != 0) {

        fprintf(stderr, "Error closing file: '%s'\n", filename);
      }
      exit(EXIT_FAILURE);
    }
  }

  if (size != 4 && size != 8 && size != 16 && size != 32 && size != 64) {
    fprintf(stderr,
            "Invalid grid size argument. Please chose a grid size among ( "
            "4 | 8 | 16 | 32 | 64 )\n");
    exit(EXIT_FAILURE);
  }

  rewind(file);

  return size;
}

void file_parser(t_grid *grid, char *filename) {

  int size = 0;
  int row = 0;
  int col = 0;
  int line = 1;
  int i = 0;
  char ch;

  FILE *file = fopen(filename, "r+");
  if (file == NULL) {
    fprintf(stderr, "Error opening file: '%s'\n", filename);
    exit(EXIT_FAILURE);
  }
  fseek(file, 0, SEEK_END);
  fprintf(file, "\n");
  fseek(file, 0, SEEK_SET);
  size = get_size(file, filename);
  grid_allocate(grid, size);

  while ((ch = fgetc(file)) != EOF) {

    if (ch == '\n') {
      line++;

      continue;
    } else if (ch == '#') {
      // Ignore comment lines
      while ((ch = fgetc(file)) != EOF && ch != '\n')
        ;

      if (ch == EOF) {
        break;
      }

    } else if (ch == ' ' || ch == '\t') {

      continue;

    }

    else if (check_char(ch)) {

      grid->grid[i] = ch;
      col++;
      i++;

      ch = fgetc(file);

      if (ch == ' ' || ch == '\t') {

        while ((ch = fgetc(file)) == ' ' || ch == '\t')
          ;
      }

      if (ch == '\n' || ch == '#') {
        ungetc(ch, file); // Put back non-newline char

        if (col != size) {

          fprintf(stderr,
                  "Error: line %d is malformed (wrong number of columns)\n",
                  row + 1);
          grid_free(grid);
          if (fclose(file) != 0) {

            fprintf(stderr, "Error closing file: '%s'\n", filename);
          }

          exit(EXIT_FAILURE);
        }
        row++;
        line++;
        col = 0;
      } else {
        ungetc(ch, file); // Put back non-newline char
      }

    } else {
      fprintf(stderr, "Error: wrong character '%c' at line %d of the file!\n",
              ch, line);
      grid_free(grid);
      if (fclose(file) != 0) {

        fprintf(stderr, "Error closing file: '%s'\n", filename);
      }
      exit(EXIT_FAILURE);
    }
  }

  if (row != size) {
    if (row < size) {
      fprintf(stderr, "Error: grid has %d missing lines!\n", size - row);
    } else {
      fprintf(stderr, "Error: grid has %d added lines!\n", row - size);
    }
    grid_free(grid);
    if (fclose(file) != 0) {

      fprintf(stderr, "Error closing file: '%s'\n", filename);
    }
    exit(EXIT_FAILURE);
  }

  if (fclose(file) != 0) {

    fprintf(stderr, "Error closing file: '%s'\n", filename);
    exit(EXIT_FAILURE);
  }
}