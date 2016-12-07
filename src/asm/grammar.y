%{

#include <stdio.h>
#include "symtable.h"

extern int yylex(void);

extern int yylineno;
extern FILE* yyin;

S_TABLE *procedures;
S_TABLE *labels;

%}

%union {
    int  num;
    char *ident;
}

%token <num> INT 
%token <ident> IDENT
%token COMMA RPAR LPAR

%token <ident> LBL PROC
%token POP IPOP PUSH IPUSH PUSHS
%token CALL RET JPC JMP
%token READ WRITE
%token RAND DUP OP HALT

%token NL

%%

program
    : %empty
    | stat NL program
    ;

stat
    : PROC {
        printf("found a procedure\n");
        add_sym_entry(procedures, $1, yylineno);
    }
    | LBL stat  { printf("found a labeled stat\n"); }
    | POP INT
    | PUSH INT
    | PUSHS INT
    | IPOP
    | IPUSH
    | CALL IDENT
    | JMP IDENT
    | JPC IDENT
    | READ INT  
    | WRITE INT
    | RAND INT
    | DUP
    | OP INT
    | HALT
    ;

%%

int main(void) {
    yyin = fopen("../../tests/test.s", "r");
    procedures = new_sym_table();
    labels = new_sym_table();

    do {
        yyparse();
    } while(!feof(yyin));

    for (int i = 0; i < procedures->size; i++)
        printf("PROCEDURE '%s' at PC = %d\n", procedures->syms[i], procedures->ofs[i]);

    return 0;
}

yyerror(const char *s) {
    printf("%d: %s\n", yylineno, s);
}