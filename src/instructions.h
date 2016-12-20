#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

typedef int16_t opcode;

// Regular instruction codes
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
#define LOAD 0x0e
#define STORE 0x0f
#define MSWAP 0x10
#define RETZ 0x11
#define RETNZ 0x12
#define HALT 0x63

#endif  // INSTRUCTIONS_H
