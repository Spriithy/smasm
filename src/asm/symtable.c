#include "symtable.h"

S_TABLE *new_sym_table() {
    S_TABLE *st = (S_TABLE *) calloc(1, sizeof(S_TABLE));
    st->syms = malloc(S_TABLE_INITIAL_SIZE * sizeof(char *));
    st->ofs = malloc(S_TABLE_INITIAL_SIZE * sizeof(int));
    st->capacity = S_TABLE_INITIAL_SIZE;
    st->size = 0;
    return st;
}

void add_sym_entry(S_TABLE *st, char *sym, int ofs) {
    if (st->capacity - 1 <= st->size) {
        st->syms = realloc(st->syms, (st->size + 2) * sizeof(char *));
        st->ofs = realloc(st->ofs, (st->size + 2) * sizeof(int));
        st->capacity += 2;
    }

    st->syms[st->size] = malloc(strlen(sym) * sizeof(char));
    strcpy(st->syms[st->size], sym);
    st->ofs[st->size] = ofs;
    st->size++;
}

int get_sym_offset(S_TABLE *st, char *sym) {
    for (int i = 0; i < st->size; i++) {
        if (strcmp(sym, st->syms[i]) == 0)
            return st->ofs[i];
    }
    return -1;
}