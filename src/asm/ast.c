#include "ast.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct node *new_node(int lno) {
    node *n = (node *) calloc(1, sizeof(node));
    n->lno = lno;
    n->next = NULL;
    return n;
}

struct node *new_ctrl(int sym, char *to, int lno) {
    node *n = new_node(lno);
    n->sym = sym;
    n->name = malloc(strlen(to) * sizeof(char));
    strcpy(n->name, to);
    return n;
}

struct node *new_instr(int sym, int arg, int lno) {
    node *n = new_node(lno);
    n->sym = sym;
    n->arg = arg;
    return n;
}

struct node *new_ph(int sym, char *name, int lno) {
    node *n = new_node(lno);
    n->sym = sym;
    n->name = malloc(strlen(name) * sizeof(char));
    strcpy(n->name, name);
    return n;
} 

void emit(node *n, S_TABLE *st, FILE* f) {
    switch (n->sym) {
    case PRC: case LBL: return;
    case POP:
    case PUSH:
    case PUSHS:
    case WRITE:
    case READ:
    case RND:
    case OP:
        fprintf(f, "%02X %02X %02X\n", n->sym, (n->arg >> 8) & 0xff, n->arg & 0xff);
        return;
    case CALL:
    case JMP:
    case JPC: {

        ofs = at - n->lno;
        printf("%d\n", ofs);
        fprintf(f, "%02X %02X %02X\n", n->sym, (ofs >> 8) & 0xff, ofs & 0xff); // TODO
        return;
    }
    default:
        fprintf(f, "%02X 00 00\n", n->sym);
    }
}
