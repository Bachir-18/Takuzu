#include "../include/takuzu.h"
#include "../include/grid.h"
#include "../include/utility.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

  globalVariables variables;
  variables.help_option = false;
  variables.all = false;
  variables.unique = false;
  variables.output = false;
  variables.generate_mode = false;
  variables.verbose = false;

  while ((variables.opt =
              getopt_long(argc, argv, "hvaug::o:", long_options, NULL)) != -1) {

    switch (variables.opt) {
    case 'h':
      variables.help_option = true;
      break;

    case 'a':
      variables.all = true;
      break;

    case 'u':
      variables.unique = true;
      break;

    case 'o':
      variables.output = true;
      variables.output_file = optarg;
      break;

    case 'g':
      variables.generate_mode = true;
      if (optarg != NULL) {
        int grid_size = atoi(optarg);
        if (grid_size == 4 || grid_size == 8 || grid_size == 16 ||
            grid_size == 32 || grid_size == 64) {
          variables.generate_size = grid_size;
        } else {
          fprintf(
              stderr,
              "Invalid grid size argument. Please chose a grid size among ( "
              "4 | 8 | 16 | 32 | 64 )");
          exit(EXIT_FAILURE);
        }
      }

      else {
        variables.generate_size = 0;
      }

      break;

    case 'v':
      variables.verbose = true;
      break;

    default:
      fprintf(stderr, "Invalid option\n");
      exit(EXIT_FAILURE);
    }
  }

  if (variables.help_option) {
    PrintHelp();
  }

  if (!variables.generate_mode) { // solver mode

    if (variables.unique) {
      fprintf(stderr,
              "takuzu: warning: option 'unique' conflicts with solver mode, "
              "exiting!\n");
      exit(EXIT_FAILURE);
    }

    if (optind >=
        argc) { // look if there is any arguments left after the options
      fprintf(stderr, "takuzu: error: no input grid given!\n");
      exit(EXIT_FAILURE);
    }
    if (variables.all) {
      t_grid grid;
      char *filename = argv[optind];
      int verbose = 0;
      if (variables.verbose) {
        verbose = 1;
      }

      file_parser(&grid, filename);

      t_grid gridCopy;
      grid_copy(&grid, &gridCopy);
      file_parser(&grid, filename);
      if (variables.output) {
        FILE *file = fopen(variables.output_file, "w");

        if (file == NULL) {
          perror("takuzu: error opening the output file\n");
          exit(EXIT_FAILURE);
        }
        printf("Output is redirecting to the file %s\n", variables.output_file);
        grid_solver(&grid, MODE_ALL, file, verbose);

        if (fclose(file) != 0) {
          perror("error closing the output file\n");
          exit(EXIT_FAILURE);
        }
      } else {
        grid_solver(&grid, MODE_ALL, stdout, verbose);
      }
    } else {
      t_grid grid;
      char *filename = argv[optind];
      int verbose = 0;
      if (variables.verbose) {
        verbose = 1;
      }

      file_parser(&grid, filename);

      t_grid gridCopy;
      grid_copy(&grid, &gridCopy);
      file_parser(&grid, filename);
      if (variables.output) {
        FILE *file = fopen(variables.output_file, "w");

        if (file == NULL) {
          perror("takuzu: error opening the output file\n");
          exit(EXIT_FAILURE);
        }
        printf("Output is redirecting to the file %s\n", variables.output_file);
        grid_solver(&grid, MODE_FIRST, file, verbose);

        if (fclose(file) != 0) {
          perror("error closing the output file\n");
          exit(EXIT_FAILURE);
        }
      } else {
        grid_solver(&grid, MODE_FIRST, stdout, verbose);
      }
    }
  }

  if (variables.generate_mode) { // generation mode
    if (variables.generate_size == 0) {
      variables.generate_size = 8;
    }

    else {
      printf("grid size = %d x %d \n", variables.generate_size,
             variables.generate_size);
    }

    if (variables.all) {

      printf("takuzu: warning: option 'all' conflicts with generation mode, "
             "disabling it!\n");
      variables.all = 0;
    }

    if (optind <
        argc) { // look if there is any arguments left after the options
      fprintf(stderr, "Generation mode: no need to provide a file\n");
      exit(EXIT_FAILURE);
    }
    int verbose = 0;
    if (variables.verbose) {
      verbose = 1;
    }
    if (variables.unique) {
      t_grid grid;
      int percentage = 20;
      printf("Searching for a grid of size %d x %d having at least one "
             "solution ...\n",
             variables.generate_size, variables.generate_size);
      if (variables.output) {
        FILE *file = fopen(variables.output_file, "w");
        if (file == NULL) {
          perror("takuzu: error opening the output file\n");
          exit(EXIT_FAILURE);
        }
        printf("Output is redirected to the file %s\n", variables.output_file);
        fprintf(file, "Generating a solved grid of size %d x %d \n",
                variables.generate_size, variables.generate_size);
        generate_grid(variables.generate_size, percentage, &grid, file, 1,
                      verbose);
        grid_print(&grid, file);
        if (fclose(file) != 0) {
          perror("error closing the output file\n");
          exit(EXIT_FAILURE);
        }
      } else {
        generate_grid(variables.generate_size, percentage, &grid, stdout, 1,
                      verbose);
        fprintf(stdout, "Grid of size %d x %d generated !!!\n",
                variables.generate_size, variables.generate_size);
        grid_print(&grid, stdout);
      }

    } else {

      t_grid g;
      int percentage = 20;
      if (variables.output) {
        FILE *file = fopen(variables.output_file, "w");

        if (file == NULL) {
          perror("takuzu: error opening the output file\n");
          exit(EXIT_FAILURE);
        }
        generate_grid(variables.generate_size, percentage, &g, file, 0,
                      verbose);
        printf("Output is redirected to the file %s\n", variables.output_file);
        fprintf(file, "Grid of size %d x %d generated !!!\n",
                variables.generate_size, variables.generate_size);
        grid_print(&g, file);
        if (fclose(file) != 0) {
          perror("error closing the file\n");
          exit(EXIT_FAILURE);
        }
      } else {
        fprintf(stdout, "Generating a grid of size %d x %d ...\n\n",
                variables.generate_size, variables.generate_size);
        generate_grid(variables.generate_size, percentage, &g, stdout, 0,
                      verbose);
        fprintf(stdout, "Grid of size %d x %d generated !!!\n",
                variables.generate_size, variables.generate_size);
        grid_print(&g, stdout);
      }
    }
  }
  return 0;
}