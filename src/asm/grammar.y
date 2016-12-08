%{

#include <stdio.h>
#include "ast.h"
#include "symtable.h"

extern int yylex(void);

extern int yylineno;

extern FILE *yyin, *yyout;

S_TABLE *labels;
S_TABLE *procedures;
node *ast_head, *ast_tail;

int pc = 0;
int errcount = 0;

char *tok;

%}

%union {
    int  num;
    char *ident;
}

%token <num> INT 
%token <ident> IDENT TOKEN_LBL TOKEN_PROC
%token <ident> TOKEN_POP TOKEN_IPOP TOKEN_PUSH TOKEN_IPUSH TOKEN_PUSHS
%token <ident> TOKEN_CALL TOKEN_RET TOKEN_JPC TOKEN_JMP
%token <ident> TOKEN_READ TOKEN_WRITE
%token <ident> TOKEN_RAND TOKEN_DUP TOKEN_OP TOKEN_HALT

%token NL

%%

program
    : %empty
    | stat NL program
    | TOKEN_LBL {
        tok = $1;
        printf("%3d: label -> %s\n", pc, $1);
        add_sym_entry(labels, $1, pc);
    } stat NL program
    | TOKEN_PROC {
        tok = $1;
        printf("%3d: procedure -> %s\n", pc, $1);
        add_sym_entry(procedures, $1, pc);
    } NL program 
    | NL program
    ;

stat
    : TOKEN_POP INT {
        tok = $1;
        printf("%3d: pop %d\n", pc, $2);
        ast_tail->next = new_instr(POP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSH INT {
        tok = $1;
        printf("%3d: push %d\n", pc, $2);
        ast_tail->next = new_instr(PUSH, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSHS INT {
        tok = $1;
        printf("%3d: push %d\n", pc, $2);
        ast_tail->next = new_instr(PUSHS, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPOP {
        tok = $1;
        printf("%3d: ipop\n", pc);
        ast_tail->next = new_instr(IPOP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPUSH {
        tok = $1;
        printf("%3d: ipush\n", pc);
        ast_tail->next = new_instr(IPUSH, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_CALL IDENT {
        tok = $2;
        printf("%3d: call %s\n", pc, $2);
        ast_tail->next = new_ctrl(CALL, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RET {
        tok = $1;
        printf("%3d: ret\n", pc);
        ast_tail->next = new_instr(RET, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JMP IDENT {
        tok = $2;
        printf("%3d: jmp %s\n", pc, $2);
        ast_tail->next = new_ctrl(JMP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JPC IDENT {
        tok = $2;
        printf("%3d: jpc %s\n", pc, $2);
        ast_tail->next = new_ctrl(JPC, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_WRITE INT {
        tok = $1;
        printf("%3d: write %d\n", pc, $2);
        ast_tail->next = new_instr(WRITE, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_READ INT {
        tok = $1;
        printf("%3d: read %d\n", pc, $2);
        ast_tail->next = new_instr(READ, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RAND INT {
        tok = $1;
        printf("%3d: rand %d\n", pc, $2);
        ast_tail->next = new_instr(RND, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_DUP {
        tok = $1;
        printf("%3d: dup\n", pc);
        ast_tail->next = new_instr(DUP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_OP INT {
        tok = $1;
        printf("%3d: op %d\n", pc, $2);
        ast_tail->next = new_instr(OP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_HALT {
        tok = $1;
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

    node *p = ast_head;
    while ((p = p->next) != NULL) {
        errcount += emit(p, labels, procedures, yyout);
    }
    if (errcount) {
        printf("encountered \x1b[1;31m%d\x1b[0m error%s\n", errcount, (errcount > 1 ? "s" : ""));
        remove("a.out");
    }

    printf("PROCEDURES:\n");
    for (int i = 0; i < procedures->size; i++)
        printf("  '%s' : %d\n", procedures->syms[i], procedures->ofs[i]);

    printf("LABELS:\n");
    for (int i = 0; i < labels->size; i++)
        printf("  '%s' : %d\n", labels->syms[i], labels->ofs[i]);

    return 0;
}

yyerror(const char *s) {
    errcount++;
    printf("\x1b[1;31merror:\x1b[0m\x1b[33m%d:\x1b[0m %s. Unexpected token after '%s'\n", yylineno, s, tok);
    return 1;
}