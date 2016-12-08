%{

#include <stdio.h>
#include "symtable.h"
#include "gen.h"

extern int yylex(void);

extern int yylineno;
extern FILE* yyin;
extern FILE* yyout;

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
        add_sym_entry(procedures, $1, yylineno);
    }
    | LBL stat { 
        add_sym_entry(labels, $1, yylineno);
    }
    | POP INT {
        gen_instr(yyout, 0, $2);
    }
    | PUSH INT {
        gen_instr(yyout, 1, $2);
    }
    | PUSHS INT {
        gen_instr(yyout, 4, $2);
    }
    | IPOP {
        gen_sym(yyout, 2);
    }
    | IPUSH {
        gen_sym(yyout, 3);
    }
    | CALL IDENT {
        gen_instr(yyout, 5, get_sym_offset(procedures, $2));
    }
    | RET {
        gen_sym(yyout, 6);
    }
    | JMP IDENT {
        gen_instr(yyout, 7, get_sym_offset(labels, $2));
    }
    | JPC IDENT {
        gen_instr(yyout, 8, get_sym_offset(labels, $2));
    }
    | WRITE INT {
        gen_instr(yyout, 9, $2);
    }
    | READ INT {
        gen_instr(yyout, 10, $2);
    }
    | RAND INT {
        gen_instr(yyout, 11, $2);
    }
    | DUP {
        gen_sym(yyout, 12);
    }
    | OP INT {
        gen_instr(yyout, 13, $2);
    }
    | HALT {
        gen_sym(yyout, 99);
    }
    ;

%%

int main(void) {
    yyin = fopen("../../tests/test.s", "r");
    yyout = fopen("a.out", "w+");

    procedures = new_sym_table();
    labels = new_sym_table();

    do {
        yyparse();
    } while(!feof(yyin));

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