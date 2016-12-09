#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include "../instructions.h"
#include "memory.h"

int execute(opcode *code);
void trace(int pc, int sp, opcode op, opcode ex, word top);

#endif  // INTERPRETER_H
