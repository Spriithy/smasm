#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include "../instructions.h"
#include "dispatch.h"
#include "log.h"
#include "memory.h"
#include "stack.h"

FILE *vmout;

/* Setting up some useful macros for readability */
#define skip(n) \
  pc += n;      \
  dispatch()

#define S_PUSH(x) stack_push(s, (x))
#define S_POP() stack_pop(s)
#define S_PEEK() s->data[s->size - 1]

#define dispatch()                      \
  trace(pc, s->size, op, ex, S_PEEK()); \
  op = code[pc];                        \
  ex = code[pc + 1];                    \
  goto *table[code[pc++]]

/* The function used to execute a chunk of bytecodes */
int execute(opcode *code) {
  vmout = fopen("vm.log", "w");
  //  if (vmout == NULL) VM_ERROR("%s", "couldn't open/create log file");

  // Working out the dispatch table
  SETUP_TABLE();

  stack *s = new_stack(STACK_SIZE_INITIAL);
  opcode op = 0, ex = 0, u, v;
  word mem[4000] = {0};
  int pc = 0;

  dispatch();
do_POP:
  u = S_POP();
  mem[ex] = u;
  skip(1);
do_PUSH:
  u = mem[ex];
  S_PUSH(u);
  skip(1);
do_IPOP:
  u = S_POP();
  v = S_POP();
  mem[u] = v;
  skip(0);
do_IPUSH:
  u = S_POP();
  v = mem[u];
  S_PUSH(v);
  skip(0);
do_PUSHS:
  S_PUSH(ex);
  skip(1);
do_CALL:
  S_PUSH(pc);
  pc = ex;
  dispatch();
do_RET:
  pc = S_POP();
  dispatch();
do_JMP:
  skip(ex - 1);
  dispatch();
do_JPC:
  if (S_POP()) {
    skip(ex - 1);
  }
  skip(1);
do_WRITE:
  printf("%hd\n", mem[ex]);
  skip(1);
do_READ:
  scanf("%hd", &mem[ex]);
  skip(1);
do_RND:
  // TODO
  skip(1);
do_DUP:
  u = S_PEEK();
  S_PUSH(u);
  skip(0);
do_OP:
  u = S_POP();
  switch (ex) {
    case 0x0:
      v = S_POP();
      S_PUSH(v == u);
      break;
    case 0x1:
      v = S_POP();
      S_PUSH(v != u);
      break;
    case 0x2:
      v = S_POP();
      S_PUSH(v > u);
      break;
    case 0x3:
      v = S_POP();
      S_PUSH(v >= u);
      break;
    case 0x4:
      v = S_POP();
      S_PUSH(v < u);
      break;
    case 0x5:
      v = S_POP();
      S_PUSH(v <= u);
      break;
    case 0x6:
      v = S_POP();
      S_PUSH(v & u);
      break;
    case 0x7:
      v = S_POP();
      S_PUSH(v | u);
      break;
    case 0x8:
      v = S_POP();
      S_PUSH(v ^ u);
      break;
    case 0x9:
      S_PUSH(~u);
      break;
    case 0xa:
      S_PUSH(-u);
      break;
    case 0xb:
      v = S_POP();
      S_PUSH(v + u);
      break;
    case 0xc:
      v = S_POP();
      S_PUSH(v - u);
      break;
    case 0xd:
      v = S_POP();
      S_PUSH(v * u);
      break;
    case 0xe:
      v = S_POP();
      if (!u) VM_ERROR("%s", "division by zero is forbidden");
      S_PUSH(v / u);
      break;
    case 0xf:
      v = S_POP();
      S_PUSH(v % u);
      break;
    default:
      VM_ERROR("unknown arithmetic operation 0x%X", ex);
  }
  skip(1);
do_ERROR:
  VM_ERROR("unknown instruction 0x%04X", op);

  fclose(vmout);
do_HALT:
  return S_PEEK();
}

void trace(int pc, int sp, opcode op, opcode ex, word top) {
  fprintf(vmout, "[log]\tpc=%d\n\tsp=%d\n\ttop=%hd\n", pc, sp, top);
  fprintf(vmout, "\texec=0x%04X next=0x%04X\n", op, ex);
}
