#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include "../instructions.h"
#include "dispatch.h"
#include "log.h"
#include "memory.h"
#include "stack.h"

FILE *vmout;

#define S_PUSH(x) stack_push(s, (x))
#define S_POP() stack_pop(s)
#define S_PEEK() s->data[s->size - 1]

/* The function used to execute a chunk of bytecodes */
int execute(opcode *code) {
  vmout = fopen("vm.log", "w");
  if (vmout == NULL) {
    VM_ERROR("%s", "couldn't open/create log file");
  }

  // Working out the dispatch table
  SETUP_TABLE();

  stack *s = new_stack(STACK_SIZE_INITIAL);
  opcode op = 0, ex = 0, u, v, w;
  word mem[4000] = {0};
  word pc = 0;

dispatch: /* will dispatch the work for the next instruction and log */
  op = code[pc];
  ex = code[pc + 1];
  trace(pc, s->size, op, ex, S_PEEK());
  goto *table[op];

do_POP:
  u = S_POP();
  mem[ex] = u;
  pc += 2;
  goto dispatch;
do_PUSH:
  u = mem[ex];
  S_PUSH(u);
  pc += 2;
  goto dispatch;
do_IPOP:
  u = S_POP();
  v = S_POP();
  mem[u] = v;
  pc++;
  goto dispatch;
do_IPUSH:
  u = S_POP();
  v = mem[u];
  S_PUSH(v);
  pc++;
  goto dispatch;
do_PUSHS:
  S_PUSH(ex);
  pc += 2;
  goto dispatch;
do_CALL:
  S_PUSH(pc);
  pc = ex;
  goto dispatch;
do_RET:
  pc = S_POP();
  goto dispatch;
do_RETZ:
  if (S_POP()) pc = S_POP();
  goto dispatch;
do_RETNZ:
  if (!S_POP()) pc = S_POP();
  goto dispatch;
do_JMP:
  pc += 2 * (ex + 1);
  goto dispatch;
do_JPC:
  if (S_POP()) {
    pc += 2 * (ex + 1);
    goto dispatch;
  }
  pc += 2;
  goto dispatch;
do_WRITE:
  printf("%hd\n", mem[ex]);
  pc += 2;
  goto dispatch;
do_READ:
  scanf("%hd", &mem[ex]);
  pc += 2;
  goto dispatch;
do_RND:
  // TODO
  pc += 2;
  goto dispatch;
do_DUP:
  u = S_PEEK();
  S_PUSH(u);
  pc++;
do_MSWAP:
  u = S_POP();
  v = S_POP();
  w = mem[u];
  mem[u] = mem[v];
  mem[v] = w;
  pc++;
  goto dispatch;
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
      if (!u) {
        VM_ERROR("%s", "division by zero is forbidden");
      }
      S_PUSH(v / u);
      break;
    case 0xf:
      v = S_POP();
      S_PUSH(v % u);
      break;
    default:
      VM_ERROR("unknown arithmetic operation 0x%X", ex);
  }
  pc += 2;
  goto dispatch;
do_ERROR:
  VM_ERROR("unknown instruction 0x%04X", op);

  fclose(vmout);
do_HALT:
  return S_PEEK();
}

void trace(int pc, int sp, opcode op, opcode ex, word top) {
  fprintf(vmout, "pc=%d:\tsp=%d, top=%hd\n", pc, sp, top);
  fprintf(vmout, "\texec=0x%04X next=0x%04X\n", op, ex);
}
