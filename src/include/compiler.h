#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include "opcodes.h"

#define COMPILER_MAX_ERRCOUNT 20

typedef struct {
  int line;
  char* message;
} error;

typedef struct {
  char* name;
  int line;
} Label;

typedef struct {
  int verbose;

  char* path;
  FILE *input, *output;

  int line;
  Label* labels;

  int errcount;
  error errors[COMPILER_MAX_ERRCOUNT];
} Compiler;

void compile(char* path, int verbose);

#endif  // COMPILER_H
