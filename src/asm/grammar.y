%{

#include <stdio.h>
#include "ast.h"
#include "symtable.h"

extern int yylex(void);

extern int yylineno;
extern FILE* yyin;
extern FILE* yyout;

S_TABLE *labels;
S_TABLE *procedures;
node *ast_head, *ast_tail;

int pc = 0;

%}

%union {
    int  num;
    char *ident;
}

%token <num> INT 
%token <ident> IDENT TOKEN_LBL TOKEN_PROC
%token TOKEN_POP TOKEN_IPOP TOKEN_PUSH TOKEN_IPUSH TOKEN_PUSHS
%token TOKEN_CALL TOKEN_RET TOKEN_JPC TOKEN_JMP
%token TOKEN_READ TOKEN_WRITE
%token TOKEN_RAND TOKEN_DUP TOKEN_OP TOKEN_HALT

%token NL

%%

program
    : %empty
    | stat NL program
    | TOKEN_LBL { 
        printf("%3d: label -> %s\n", pc, $1);
        add_sym_entry(labels, $1, pc);
    } stat NL program
    ;

stat
    : TOKEN_PROC {
        printf("%3d: procedure -> %s\n", pc, $1);
        add_sym_entry(procedures, $1, pc);
    }
    | TOKEN_POP INT {
        printf("%3d: pop %d\n", pc, $2);
        ast_tail->next = new_instr(POP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSH INT {
        printf("%3d: push %d\n", pc, $2);
        ast_tail->next = new_instr(PUSH, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSHS INT {
        printf("%3d: push %d\n", pc, $2);
        ast_tail->next = new_instr(PUSHS, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPOP {
        printf("%3d: ipop\n", pc);
        ast_tail->next = new_instr(IPOP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPUSH {
        printf("%3d: ipush\n", pc);
        ast_tail->next = new_instr(IPUSH, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_CALL IDENT {
        printf("%3d: call %s\n", pc, $2);
        ast_tail->next = new_ctrl(CALL, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RET {
        printf("%3d: ret\n", pc);
        ast_tail->next = new_instr(RET, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JMP IDENT {
        printf("%3d: jmp %s\n", pc, $2);
        ast_tail->next = new_ctrl(JMP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JPC IDENT {
        printf("%3d: jpc %s\n", pc, $2);
        ast_tail->next = new_ctrl(JPC, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_WRITE INT {
        printf("%3d: write %d\n", pc, $2);
        ast_tail->next = new_instr(WRITE, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_READ INT {
        printf("%3d: read %d\n", pc, $2);
        ast_tail->next = new_instr(READ, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RAND INT {
        printf("%3d: rand %d\n", pc, $2);
        ast_tail->next = new_instr(RND, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_DUP {
        printf("%3d: dup\n", pc);
        ast_tail->next = new_instr(DUP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_OP INT {
        printf("%3d: op %d\n", pc, $2);
        ast_tail->next = new_instr(OP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_HALT {
        printf("%3d: halt\n", pc);
        ast_tail->next = new_instr(HALT, 0, pc++);
        ast_tail = ast_tail->next;
    }
    ;

%%

int main(void) {
    yyin = fopen("../../tests/test.s", "r");
    yyout = fopen("a.out", "w+");

    ast_head = new_ph(VOID, "<start>", 0);
    ast_tail = ast_head;
    procedures = new_sym_table();
    labels = new_sym_table();

    do {
        yyparse();
    } while(!feof(yyin));

    int errcount = 0;
    node *p = ast_head;
    while ((p = p->next) != NULL) {
        errcount += emit(p, labels, procedures, yyout);
    }
    printf("encountered \x1b[1;31m%d\x1b[0m error%s\n", errcount, (errcount > 1 ? "s" : ""));
    if (errcount) remove("a.out");

    printf("PROCEDURES:\n");
    for (int i = 0; i < procedures->size; i++)
        printf("  '%s' : %d\n", procedures->syms[i], procedures->ofs[i]);

    printf("LABELS:\n");
    for (int i = 0; i < labels->size; i++)
        printf("  '%s' : %d\n", labels->syms[i], labels->ofs[i]);

    return 0;
}

yyerror(const char *s) {
    printf("%d: %s\n", yylineno, s);
    return 1;
}