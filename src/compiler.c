#include "include/compiler.h"
#include <stdio.h>
#include <string.h>
#include "include/colors.h"

#define IS_LABEL(s) (s[strlen(s) - 1] == ':' ? 1 : 0)

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

  if (file == NULL) {
    printf(COLOR_RED "error: " COLOR_NONE "no such file: '%s'\n", c->path);
    exit(1);
  }

  int ln = 1;
  char line[256] = {0}, tok[32] = {0};

  while (fgets(line, 256, file) != NULL) {
    sscanf(line, " %s", tok);

    if (IS_LABEL(tok)) {
      tok[strlen(tok) - 1] = 0;
      printf("Label '%s' on line %d\n", tok, ln);
    }
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

  if (c->input == NULL) {
    printf(COLOR_RED "error: " COLOR_NONE "no such file: '%s'\n", c->path);
    exit(1);
  }
  fclose(c->input);

  c->verbose = verbose;

  parse_labels(c);
}
