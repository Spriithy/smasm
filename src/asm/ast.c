#include "ast.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../colors.h"
#include "../instructions.h"

struct node *new_node(int lno) {
  struct node *n = calloc(1, sizeof(struct node));
  n->lno = lno;
  n->next = NULL;
  return n;
}

struct node *new_ctrl(int sym, char *to, int lno) {
  struct node *n = new_node(lno);
  n->sym = sym;
  n->name = strdup(to);
  return n;
}

struct node *new_instr(int sym, int arg, int lno) {
  struct node *n = new_node(lno);
  n->sym = sym;
  n->arg = arg;
  return n;
}

struct node *new_ph(int sym, char *name, int lno) {
  struct node *n = new_node(lno);
  n->sym = sym;
  n->name = strdup(name);
  return n;
}

int emit(struct node *n, S_TABLE *lbls, FILE *f) {
  int ofs = 0;

  switch (n->sym) {
    case LBL:
      return 0;
    case POP:
    case PUSH:
    case PUSHS:
    case WRITE:
    case READ:
    case RND:
    case OP:
    case -JMP:
    case -JPC:
      fprintf(f, "%02X %02X %02X\n", abs(n->sym), (n->arg >> 8) & 0xff,
              n->arg & 0xff);
      return 0;
    case CALL:
    case JMP:
    case JPC:
      if ((ofs = get_sym_offset(lbls, n->name)) == -1) {
        printf(COLOR_RED "error:" COLOR_YELLOW "%d:" COLOR_NONE
                         " reference to undefined label " COLOR_GREEN
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
