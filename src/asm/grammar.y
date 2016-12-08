%{

#include <stdio.h>
#include "ast.h"

extern int yylex(void);

extern int yylineno;
extern FILE* yyin;
extern FILE* yyout;

S_TABLE *labels; // TODO finish implementation
node *ast_head, *ast_tail;

%}

%union {
    int  num;
    char *ident;
}

%token <num> INT 
%token <ident> IDENT

%token <ident> TOKEN_LBL TOKEN_PROC
%token TOKEN_POP TOKEN_IPOP TOKEN_PUSH TOKEN_IPUSH TOKEN_PUSHS
%token TOKEN_CALL TOKEN_RET TOKEN_JPC TOKEN_JMP
%token TOKEN_READ TOKEN_WRITE
%token TOKEN_RAND TOKEN_DUP TOKEN_OP TOKEN_HALT

%token NL

%%

program
    : %empty
    | stat NL program
    ;

stat
    : TOKEN_PROC {
        ast_tail->next = new_ph(PRC, $1, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_LBL stat { 
        ast_tail->next = new_ph(LBL, $1, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_POP INT {
        ast_tail->next = new_instr(POP, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSH INT {
        ast_tail->next = new_instr(PUSH, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSHS INT {
        ast_tail->next = new_instr(PUSHS, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPOP {
        ast_tail->next = new_instr(IPOP, 0, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPUSH {
        ast_tail->next = new_instr(IPUSH, 0, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_CALL IDENT {
        ast_tail->next = new_ctrl(CALL, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RET {
        ast_tail->next = new_instr(RET, 0, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JMP IDENT {
        ast_tail->next = new_ctrl(JMP, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JPC IDENT {
        ast_tail->next = new_ctrl(JPC, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_WRITE INT {
        ast_tail->next = new_instr(WRITE, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_READ INT {
        ast_tail->next = new_instr(READ, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RAND INT {
        ast_tail->next = new_instr(RND, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_DUP {
        ast_tail->next = new_instr(DUP, 0, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_OP INT {
        ast_tail->next = new_instr(OP, $2, yylineno);
        ast_tail = ast_tail->next;
    }
    | TOKEN_HALT {
        ast_tail->next = new_instr(HALT, 0, yylineno);
        ast_tail = ast_tail->next;
    }
    ;

%%

int main(void) {
    yyin = fopen("../../tests/test.s", "r");
    yyout = fopen("a.out", "w+");

    ast_head = new_ph(VOID, "<start>", 0);
    ast_tail = ast_head;

    do {
        yyparse();
    } while(!feof(yyin));

    node *p = ast_head;
    while ((p = p->next) != NULL)
        emit(p, ast_head, yyout);

    return 0;
}

yyerror(const char *s) {
    printf("%d: %s\n", yylineno, s);
    return 1;
}