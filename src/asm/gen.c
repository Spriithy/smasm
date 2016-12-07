#include "gen.h"

void gen_sym(FILE *f, int sym) {
    fprintf(f, "%02X", sym);
}

void gen_instr(FILE *f, int op, int extra) {
    fprintf(f, "%02X %02X %02X\n", op, (extra >> 8) & 0xf, extra & 0xf);
}
