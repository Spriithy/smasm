%{

#include <stdio.h>

%}

%token DOT
%token COLON

%token IDENT
%token INT

%token POP
%token PUSH
%token IPOP
%token IPUSH
%token PUSHS
%token CALL
%token RET
%token JMP
%token JPC
%token WRITE
%token READ
%token RAND
%token DUP
%token OP
%token HALT

%%

program
  : stat
  | program stat
  ;

stat
  : DOT IDENT     { printf("matched procedure\n"); }
  | IDENT COLON   { printf("matche label\n");      }
  | immediate
  | control
  ;

control
  : CALL IDENT
  | JMP IDENT
  | JPC IDENT
  ;

immediate
  : POP INT
  | PUSH INT
  | PUSHS INT
  | WRITE INT
  | READ INT
  | RAND INT
  | OP INT
  ;

%%

extern FILE *yyin;

int main(void) {


  do {
    yyparse();
  } while(!feof(yyin));

  return 0;
}


yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
}

