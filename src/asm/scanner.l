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

-?[0-9]+ {
    sscanf(yytext, "%d", &yylval.num);
    return INT;
}

0[xX][0-9a-fA-F]+ {
    sscanf(yytext, "%x", &yylval.num);
    return INT;
}

[a-zA-Z_][a-zA-Z0-9_]*":" {
    yytext[strlen(yytext) - 1] = 0;
    yylval.ident = strdup(yytext);

    return TOKEN_LBL;
}

pop {
    return TOKEN_POP;
}

push"#" {
    return TOKEN_PUSHS;
}

push {
    return TOKEN_PUSH;
}

i[pP]op {
    return TOKEN_IPOP;
}

i[pP]ush {
    return TOKEN_IPUSH;
}

call {
    return TOKEN_CALL;
}

ret {
    return TOKEN_RET;
}

retz {
  return TOKEN_RETZ;
}

retnz {
  return TOKEN_RETNZ;
}

jmp {
    return TOKEN_JMP;
}

jpc {
    return TOKEN_JPC;
}

write {
    return TOKEN_WRITE;
}

read {
    return TOKEN_READ;
}

putc {
    return TOKEN_PUTC;
}

getc {
    return TOKEN_GETC;
}

rnd {
    return TOKEN_RAND;
}

dup {
    return TOKEN_DUP;
}

mswap {
  return TOKEN_MSWAP;
}

op {
    return TOKEN_OP;
}

halt {
    return TOKEN_HALT;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.ident = strdup(yytext);
    return IDENT;
}

%%
