#ifndef TAKUZU_H
#define TAKUZU_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int opt;
  bool help_option;
  bool verbose;
  char *output_file;
  bool all;
  bool unique;
  bool output;
  bool generate_mode;
  int generate_size;
} globalVariables;

static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {"output", required_argument, NULL, 'o'},
    {"all", no_argument, NULL, 'a'},
    {"generate", optional_argument, NULL, 'g'},
    {"unique", no_argument, NULL, 'u'},
    {NULL, 0, NULL, 0}};

#endif /* TAKUZU_H */
