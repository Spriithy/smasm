#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

node *new_node(int lno) {
    node *n = calloc(1, sizeof(node));
    n->lno = lno;
    n->next = NULL;
    return n;
}

node *new_ctrl(int sym, char *to, int lno) {
    node *n = new_node(lno);
    n->sym = sym;
    n->name = malloc(strlen(to) * sizeof(char));
    strcpy(n->name, to);
    return n;
}

node *new_instr(int sym, int arg, int lno) {
    node *n = new_node(lno);
    n->sym = sym;
    n->arg = arg;
    return n;
}

node *new_ph(int sym, char *name, int lno) {
    node *n = new_node(lno);
    n->sym = sym;
    n->name = malloc(strlen(name) * sizeof(char));
    strcpy(n->name, name);
    return n;
} 

void emit(node *n, S_TABLE *lbls, S_TABLE *prcs, FILE* f) {
    int ofs = 0;
    
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
    case CALL: {
        if ((ofs = get_sym_offset(prcs, n->name)) == -1)
            printf("error:%d: calling undefined procedure '%s'\n", n->lno, n->name);

        printf("%d for %s\n", ofs, n->name);
        fprintf(f, "%02X %02X %02X\n", n->sym, (ofs >> 8) & 0xff, ofs & 0xff); // TODO
        return;
    }
    case JMP: case JPC: {
        if ((ofs = get_sym_offset(lbls, n->name)) == -1)
            printf("error:%d: jumping to undefined label '%s'\n", n->lno, n->name);
        
        ofs -= n->lno + 1;
        printf("%s: from %d to %d\n", n->name, n->lno, ofs);
        fprintf(f, "%02X %02X %02X\n", n->sym, (ofs >> 8) & 0xff, ofs & 0xff); // TODO
        return;
    }
    default:
        fprintf(f, "%02X 00 00\n", n->sym);
    }
}