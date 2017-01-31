#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
int execute(opcode *code, int log) {
  if (log) {
    vmout = fopen("vm.log", "w");
    if (vmout == NULL) {
      VM_ERROR("%s", "couldn't open/create log file");
    }
  }

  // Working out the dispatch table
  SETUP_TABLE();

  stack *s = new_stack(STACK_SIZE_INITIAL);
  opcode op = 0, ex = 0, u, v, w;
  word mem[4000] = {0};
  word pc = 0;
  char buf[3];
  time_t t;

dispatch: /* will dispatch the work for the next instruction and log */
  op = code[pc];
  ex = code[pc + 1];
  if (log) trace(pc, s->size, op, ex, S_PEEK());
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
do_STORE:
  u = S_POP();
  v = S_POP();
  mem[u] = v;
  pc += 2;
  goto dispatch;
do_IPUSH:
do_LOAD:
  u = S_POP();
  v = mem[u];
  S_PUSH(v);
  pc += 2;
  goto dispatch;
do_PUSHS:
  S_PUSH(ex);
  pc += 2;
  goto dispatch;
do_CALL:
  if (log) fprintf(vmout, "CALLING PROCEDURE: offset %d\n", ex + 1);
  S_PUSH(pc);
  pc += 2 * (ex + 1);
  goto dispatch;
do_RET:
  if (log)
    fprintf(vmout, "RETURNING FROM PROCEDURE: back to PC=%d\n",
            S_PEEK() / 2 + 1);
  pc = S_POP() + 2;
  goto dispatch;
do_RETZ:
  if (S_POP()) goto do_RET;
  goto dispatch;
do_RETNZ:
  if (!S_POP()) goto do_RET;
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
  printf("%hd", mem[ex]);
  pc += 2;
  goto dispatch;
do_READ:
  scanf("%hd", &mem[ex]);
  pc += 2;
  goto dispatch;
do_GETC:
  fgets(buf, 3, stdin);
  S_PUSH(buf[0]);
  pc += 2;
  goto dispatch;
do_PUTC:
  printf("%c", S_POP());
  pc += 2;
  goto dispatch;
do_RND:
  srand((unsigned)time(&t));
  S_PUSH(rand() % (ex - 1));
  pc += 2;
  goto dispatch;
do_DUP:
  u = S_PEEK();
  S_PUSH(u);
  pc += 2;
  goto dispatch;
do_MSWAP:
  u = S_POP();
  v = S_POP();
  w = mem[u];
  mem[u] = mem[v];
  mem[v] = w;
  pc += 2;
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

  if (log) fclose(vmout);
do_HALT:
  return S_PEEK();
}

void trace(int pc, int sp, opcode op, opcode ex, word top) {
  fprintf(vmout, "pc=%-4d\tsp=%d, top=%hd\n", pc / 2, sp, top);
  // fprintf(vmout, "\texec=0x%04X(0x%04X=%d)\n", op, ex, ex);
}
