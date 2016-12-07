%{

#include <stdio.h>
#include <stdlib.h>
#include "grammar.tab.h"

%}

%option yylineno
%option noyywrap

%%

";".*   ;
[ \t\r] ;
  
\n {
    return NL;
}

[0-9]+ {
    sscanf(yytext, "%d", &yylval.num);
    return INT;
}

0[xX][0-9a-fA-F]+ {
    sscanf(yytext, "%x", &yylval.num);
    return INT;
}

pop     {
    return POP;
}

push"#" {
    return PUSHS;
}

push {
    return PUSH;
}

ipop {
    return IPOP;
}

ipush {
    return IPUSH;
}

call {
    return CALL;
}

ret {
    return RET;
}

jmp {
    return JMP;
}

jpc {
    return JPC;
}

write {
    return WRITE;
}

read {
    return READ;
}

rand {
    return RAND;
}

dup {
    return DUP;
}

op {
    return OP;
}

halt {
    return HALT;
}

^"."[a-zA-Z_][a-zA-Z0-9_]* {
    yytext++;
    yylval.ident = malloc(strlen(yytext) * sizeof(char));
    strcpy(yylval.ident, yytext);

    return PROC;
}

^[a-zA-Z_][a-zA-Z0-9_]*":" {
    yytext[strlen(yytext) - 1] = 0;
    yylval.ident = malloc(strlen(yytext) * sizeof(char));
    strcpy(yylval.ident, yytext);

    return LBL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.ident = malloc(strlen(yytext) * sizeof(char));
    strcpy(yylval.ident, yytext);
    return IDENT;
}

%%
