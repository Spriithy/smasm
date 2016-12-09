#include "interpreter.h"

int main(int argc, char **argv) {
  opcode source[] = {
      READ, 1000, PUSH, 1000, PUSHS, 0,     OP,   0,   JPC, 12,   PUSH,
      1000, OP,   10,   POP,  1000,  WRITE, 1000, JMP, -18, HALT,
  };

  return execute(source);
}
