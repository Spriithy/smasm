#include "include/interpreter.h"
#include "include/colors.h"

static void grow(Interpreter *itp) {
  itp->size += GROWING_STACK_SIZE;
  itp->stack = realloc(itp->stack, itp->size * sizeof(word));
}

Interpreter *new_interpreter(opcode *code) {
  Interpreter *itp = calloc(1, sizeof(Interpreter));
  itp->code = code;
  itp->pc = 0;
  itp->sp = 0;
  itp->stack = malloc(INITIAL_STACK_SIZE * sizeof(word));
  itp->size = INITIAL_STACK_SIZE;
  return itp;
}

int itp_execute(Interpreter *itp) {
  opcode o = itp->code[0];
  opcode imm = 0;
  for (itp->pc = 0; o != HALT; itp->pc++) {
    o = itp->code[itp->pc];
    imm = itp->code[itp->pc + 1];
    switch (o) {
      case POP:
        pop(imm);
      case PUSH:
        push(imm);
      case IPOP:
        ipop();
      case IPUSH:
        ipush();
      case PUSHS:
        pushs();
      case CALL:
        call(imm);
      case RET:
        ret();
      case JMP:
        jmp(imm);
      case JPC:
        jpc(imm);
      case WRITE:
        write(imm);
      case READ:
        read(imm);
      case RND:
        rnd(imm);
      case DUP:
        dup();
      case OP:
        op(imm);
      case HALT:
        halt();
    }
    itp->pc++;
  }
  return spop();
}

word itp_pop(Interpreter *itp) {
  if (itp->sp <= 0) ERROR("stack is empty");
  return itp->stack[--itp->sp];
}

void itp_push(Interpreter *itp, word x) {
  if (itp->size - itp->sp <= 1) grow(itp);
  itp->stack[itp->sp++] = x;
}
