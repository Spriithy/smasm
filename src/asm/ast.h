#ifndef AST_H
#define AST_H

#include <stdio.h>
#include "symtable.h"

#define VOID 0xff
#define PRC -0x02
#define LBL -0x01

typedef struct {
    int lno;
    int sym;
    union {
        int arg;
        char *name;
    };
    struct node *next;
} node;

node *new_node(int lno);
node *new_ctrl(int sym, char *to, int lno);
node *new_instr(int sym, int arg, int lno);
node *new_ph(int sym, char *name, int lno);

int emit(node *n, S_TABLE* lbls, S_TABLE *prcs, FILE* f);

#endif // AST_H