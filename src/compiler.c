#include "include/compiler.h"
#include <stdio.h>
#include <string.h>
#include "include/colors.h"

static void gen_error(Compiler* c, char* message) {
  if (c->errcount >= COMPILER_MAX_ERRCOUNT) {
    ERROR("too many errors:");
    for (int i = 0; i < c->errcount - 1; i++) {
      ERROR(c->errors[i].message);
    }
  }
  c->errors[c->errcount++] = (error){c->line, message};
}

static void parse_labels(Compiler* c) {
  FILE* file = fopen(c->path, "r");
  char line[80], lbl[80];
  int ln = 0;
  while ((fgets(line, 80, file)) != NULL) {
    printf(COLOR_BLUE "%s:" COLOR_YELLOW "%03d" COLOR_NONE ": %s", c->path, ln,
           line);
    ln++;
  }
  fclose(file);
}

void compile(char* path, int verbose) {
  Compiler* c = calloc(1, sizeof(Compiler));
  c->path = path;
  c->errcount = 0;
  c->line = 1;
  c->input = fopen(path, "r");
  c->output = fopen("a.out", "w+");
  c->verbose = verbose;
  parse_labels(c);
}
