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

  c->labels = malloc(1 * sizeof(Label));

  while (fgets(line, 256, file) != NULL) {
    sscanf(line, " %s", tok);

    if (IS_LABEL(tok)) {
      tok[strlen(tok) - 1] = 0;
      c->labels[c->lblcount] = (Label){ln, NULL};
      c->labels[c->lblcount].name = (char*)malloc(strlen(tok) * sizeof(char));
      strcpy(c->labels[c->lblcount].name, tok);
      c->lblcount++;
      c->labels = (Label*)realloc(c->labels, (c->lblcount + 1) * sizeof(Label));
    }
    ln++;
  }

  fclose(file);

  for (int i = 0; i < c->lblcount; i++)
    printf("%d: '%s'\n", c->labels[i].line, c->labels[i].name);
}

void compile(char* path, int verbose) {
  Compiler* c = calloc(1, sizeof(Compiler));
  c->verbose = verbose;
  c->errcount = 0;
  c->path = path;
  c->line = 1;

  parse_labels(c);
}
