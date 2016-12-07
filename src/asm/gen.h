#ifndef GEN_H
#define GEN_H

#include <stdio.h>

void gen_sym(FILE *f, int sym);
void gen_instr(FILE *f, int op, int extra);

#endif // GEN_H