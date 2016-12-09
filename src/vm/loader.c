#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

opcode *load_file(char *path) {
  // Open the source file and read from it
  FILE *f = fopen(path, "r");

  if (f == NULL) {
    VM_ERROR("couldn't open/read source file '" COLOR_GREEN "%s" COLOR_NONE "'",
             path);
  }

  // Phase one: Get the count of opcodes that will be generated
  int count = 0, buf;

  while ((fscanf(f, "%X", &buf)) != EOF) count++;

  // Phase two: read the opcodes and assemble immediates values
  opcode *ops = malloc((count / 3) * sizeof(opcode));
  opcode op, ex0, ex1;
  count = 0;

  while ((fscanf(f, "%hX %hX %hX", &op, &ex0, &ex1) != EOF)) {
    ops[count++] = op;
    ops[count++] = ((ex0 & 0xff) << 8) | (ex1 & 0xff);
  }

  fclose(f);

  // and return
  return ops;
}
