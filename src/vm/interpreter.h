#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "opcodes.h"

#define INITIAL_STACK_SIZE 1 << 8
#define GROWING_STACK_SIZE 1 << 6

typedef int16_t word;

typedef struct {
  opcode *code;
  int pc;
  int sp;
  int size;
  word mem[4000];
  word *stack;
} Interpreter;

Interpreter *new_interpreter(opcode *code);
int itp_execute(Interpreter *itp);
word itp_pop(Interpreter *itp);
void itp_push(Interpreter *ipt, word x);

// Performs a simple pop, without storing the result yet
#define spop() itp_pop(itp)
#define pop(x) itp->mem[x] = spop()
#define push(x) itp_push(itp, x)
#define ipop() itp->mem[spop()] = spop()
#define ipush() push(itp->mem[spop()])
#define pushs() \
  push(imm);    \
  itp->pc++
#define call(adr)      \
  push((word)itp->pc); \
  itp->pc += adr
#define ret() itp->pc = spop()
#define jmp(adr) itp->pc += adr
#define jpc(adr) \
  if (spop()) itp->pc += (int)adr
#define write(x)             \
  printf("%d", itp->mem[x]); \
  itp->pc++
#define read(x)                \
  scanf("%d\n", &itp->mem[x]); \
  itp->pc++
#define rnd(x)      \
  push(rand() % x); \
  itp->pc++
#define dup() push(itp->stack[itp->sp - 1])
#define op(i)                 \
  itp->pc++;                  \
  switch (i) {                \
    case EQ:                  \
      push(spop() == spop()); \
    case NEQ:                 \
      push(spop() != spop()); \
    case GT:                  \
      push(spop() > spop());  \
    case GEQ:                 \
      push(spop() >= spop()); \
    case LT:                  \
      push(spop() < spop());  \
    case LEQ:                 \
      push(spop() <= spop()); \
    case AND:                 \
      push(spop() & spop());  \
    case IOR:                 \
      push(spop() | spop());  \
    case XOR:                 \
      push(spop() ^ spop());  \
    case NOT:                 \
      push(~spop());          \
    case NEG:                 \
      push(-spop());          \
    case ADD:                 \
      push(spop() + spop());  \
    case SUB:                 \
      push(spop() - spop());  \
    case MUL:                 \
      push(spop() * spop());  \
    case DIV:                 \
      push(spop() / spop());  \
    case REM:                 \
      push(spop() % spop());  \
  }
#define halt() return spop();

#endif  // INTERPRETER_H
