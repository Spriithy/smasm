#include "ast.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../colors.h"
#include "../instructions.h"

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

int emit(node *n, S_TABLE *lbls, S_TABLE *prcs, FILE *f) {
  int ofs = 0;

  switch (n->sym) {
    case PRC:
    case LBL:
      return 0;
    case POP:
    case PUSH:
    case PUSHS:
    case WRITE:
    case READ:
    case RND:
    case OP:
      fprintf(f, "%02X %02X %02X\n", n->sym, (n->arg >> 8) & 0xff,
              n->arg & 0xff);
      return 0;
    case CALL:
      if ((ofs = get_sym_offset(prcs, n->name)) == -1) {
        printf(COLOR_RED "error:" COLOR_YELLOW "%d:" COLOR_NONE
                         " calling undefined procedure " COLOR_GREEN
                         "%s" COLOR_NONE "\n",
               n->lno, n->name);
        return 1;
      }

      fprintf(f, "%02X %02X %02X\n", n->sym, (ofs >> 8) & 0xff,
              ofs & 0xff);  // TODO
      return 0;
    case JMP:
    case JPC:
      if ((ofs = get_sym_offset(lbls, n->name)) == -1) {
        printf(COLOR_RED "error:" COLOR_YELLOW "%d:" COLOR_NONE
                         " jumping to undefined label " COLOR_GREEN
                         "%s" COLOR_NONE "\n",
               n->lno, n->name);
        return 1;
      }

      ofs -= n->lno + 1;
      fprintf(f, "%02X %02X %02X\n", n->sym, (ofs >> 8) & 0xff,
              ofs & 0xff);  // TODO
      return 0;
    default:
      fprintf(f, "%02X 00 00\n", n->sym);
  }
  return 0;
}
