%{

#include <stdio.h>

%}

%union {
    int  num;
    char *ident;
}

%token <num> INT 
%token <ident> IDENT
%token COMMA RPAR LPAR

%token LBL PROC
%token POP IPOP PUSH IPUSH PUSHS
%token CALL RET JPC JMP
%token READ WRITE
%token RAND DUP OP HALT

%token NL

%%

program
    :
    | stat NL program
    ;

stat
    : PROC      { printf("found a procedure\n"); }
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

extern int yylineno;

extern FILE* yyin;

int main(void) {
    yyin = fopen("../../test.s", "r");
    do {
        yyparse();
    } while(!feof(yyin));
    return 0;
}

int yyerror(char *s) {
    printf("%d: %s\n", yylineno, s);
    return 1;
}