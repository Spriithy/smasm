#ifndef DISPATCH_H
#define DISPATCH_H

#define SETUP_TABLE()                                          \
  static void *table[100] = {                                  \
      &&do_POP,  &&do_PUSH, &&do_IPOP, &&do_IPUSH, &&do_PUSHS, \
      &&do_CALL, &&do_RET,  &&do_JMP,  &&do_JPC,   &&do_WRITE, \
      &&do_READ, &&do_RND,  &&do_DUP,  &&do_OP,                \
  };                                                           \
  for (int k = OP + 1; k < 99; k++) table[k] = &&do_ERROR;     \
  table[99] = &&do_HALT

#endif  // DISPATCH_H
