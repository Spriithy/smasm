#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

typedef int16_t opcode;

#define POP 0x00
#define PUSH 0x01
#define IPOP 0x02
#define IPUSH 0x03
#define PUSHS 0x04
#define CALL 0x05
#define RET 0x06
#define JMP 0x07
#define JPC 0x08
#define WRITE 0x09
#define READ 0x0a
#define RND 0x0b
#define DUP 0x0c
#define OP 0x0d
#define HALT 0x63

#define EQ 0x0
#define NEQ 0x1
#define GT 0x2
#define GEQ 0x3
#define LT 0x4
#define LEQ 0x5
#define AND 0x6
#define IOR 0x7
#define XOR 0x8
#define NOT 0x9
#define NEG 0xa
#define ADD 0xb
#define SUB 0xc
#define MUL 0xd
#define DIV 0xe
#define REM 0xf

char *op_name(opcode op);

#endif  // OPCODES_H
