#include "include/opcodes.h"
#include <stdlib.h>

#define CASE(x, s) \
  case x:          \
    return s

char *op_name(opcode op) {
  switch (op) {
    CASE(POP, "POP");
    CASE(PUSH, "PUSH");
    CASE(IPOP, "IPOP");
    CASE(IPUSH, "IPUSH");
    CASE(PUSHS, "PUSHS");
    CASE(CALL, "CALL");
    CASE(RET, "RET");
    CASE(JMP, "JMP");
    CASE(JPC, "JPC");
    CASE(WRITE, "WRITE");
    CASE(READ, "READ");
    CASE(RND, "RND");
    CASE(DUP, "DUP");
    CASE(OP, "OP");
    CASE(HALT, "HALT");
  }
  return "?";
}
