#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For readability purposes
#define S_TABLE sym_table

#define S_TABLE_INITIAL_SIZE 16

typedef struct {
  char **syms;
  int *ofs;
  int size;
  int capacity;
} S_TABLE;

S_TABLE *new_sym_table();
void add_sym_entry(S_TABLE *st, char *sym, int ofs);
int get_sym_offset(S_TABLE *st, char *sym);

#endif  // PROCEDURE_H
