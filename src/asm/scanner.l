%{

#include <stdio.h>
#include <stdlib.h>
#include "grammar.tab.h"

int linecount;

%}

NUMBER [0-9]+
IDENT  [a-zA-Z_][a-zA-Z0-9_]*

%%

";".*   { printf("nice comment (%s)\n", yytext + 1); }

[ \t\r] ;

"."     { printf("nice dot\n"); }
":"     { printf("nice colon\n"); }
\n      { printf("new line\n"); linecount++; return NL; }

[0-9]+  { printf("nice number %s\n", yytext); return NUM; }

pop     { printf("POP\n"); return POP; }
push"#" { printf("PUSHS\n"); return PUSHS; }
push    { printf("PUSH\n"); return PUSH; }
ipop    { printf("IPOP\n"); return IPOP; }
ipush   { printf("IPUSH\n"); return IPUSH; }
call    { printf("CALL\n"); return CALL; }
ret     { printf("RET\n"); return RET; }
jmp     { printf("JMP\n"); return JMP; }
jpc     { printf("JPC\n"); return JPC; }
write   { printf("WRITE\n"); return WRITE; }
read    { printf("READ\n"); return READ; }
rand    { printf("RAND\n"); return RAND; }
dup     { printf("DUP\n"); return DUP; }
op      { printf("OP\n"); return OP; }
halt    { printf("HALT\n"); return HALT; }

^"."[a-zA-Z_][a-zA-Z0-9_]* {
    printf("nice procedure (%s)\n", yytext + 1);
    return PROC;
}

^[a-zA-Z_][a-zA-Z0-9_]*":" {
    yytext[strlen(yytext) - 1] = 0;
    printf("nice label (%s)\n", yytext);
    return LBL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("nice identifier (%s)\n", yytext);
    return IDENT;
}

%%