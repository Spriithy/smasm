#ifndef AST_H
#define AST_H

#include <stdio.h>

#define VOID 0xff
#define PRC -0x02
#define LBL -0x01

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
#define HALT 0x63

typedef struct {
    int lno;
    int sym;
    union {
        int arg;
        char *name;
    };
    struct node *next;
} node;

struct node *new_node(int lno);
struct node *new_ctrl(int sym, char *to, int lno);
struct node *new_instr(int sym, int arg, int lno);
struct node *new_ph(int sym, char *name, int lno);

void emit(node *n, node *h, FILE* f);

#endif // AST_H