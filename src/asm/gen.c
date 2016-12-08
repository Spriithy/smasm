#include "gen.h"

void gen_sym(FILE *f, int sym) {
    gen_instr(f, sym, 0);
}

void gen_instr(FILE *f, int op, int extra) {
    fprintf(f, "%02X %02X %02X\n", op, (extra >> 8) & 0xff, extra & 0xff);
}
