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
    : stat program
    ;

stat
    :
    ;

%%

extern FILE* yyin;

int main(void) {
    yyin = fopen("../../test.s", "r");
    do {
        yyparse();
    } while(!feof(yyin));
    return 0;
}

int yyerror(char *s) {
    printf("%s\n", s);
    return 1;
}