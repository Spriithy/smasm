%{

#include <stdio.h>
#include <stdlib.h>
#include "grammar.tab.h"

int linecount;

%}

%option noyywrap

HEX    0[xX][a-fA-F0-9]+
NUMBER [0-9]+
IDENT  [a-zA-Z_][a-zA-Z0-9_]*

%%

";".*   ;
[ \t\r] ;
  
\n {
    linecount++;
    return NL;
}

NUMBER {
    sscanf(yytext, "%d", &yylval.num);
    return INT;
}

HEX {
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
    strcpy(yylval.ident, yytext);
    return PROC;
}

^[a-zA-Z_][a-zA-Z0-9_]*":" {
    yytext[strlen(yytext) - 1] = 0;
    strcpy(yylval.ident, yytext);
    return LBL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    strcpy(yylval.ident, yytext);
    return IDENT;
}

%%
