%{

#include <stdio.h>
#include <stdlib.h>

int linecount;

%}

NUMBER [0-9]+
IDENT  [a-zA-Z_][a-zA-Z0-9_]*

%%

";".*   { printf("nice comment (%s)\n", yytext + 1); }

[ \t\r] ;

"."     { printf("nice dot\n"); }
":"     { printf("nice colon\n"); }
\n      { printf("new line\n"); linecount++; }

[0-9]+  { printf("nice number %s\n", yytext); }

pop     { printf("POP\n"); }
push"#" { printf("PUSHS\n"); }
push    { printf("PUSH\n"); }
ipop    { printf("IPOP\n"); }
ipush   { printf("IPUSH\n"); }
call    { printf("CALL\n"); }
ret     { printf("RET\n"); }
jmp     { printf("JMP\n"); }
jpc     { printf("JPC\n"); }
write   { printf("WRITE\n"); }
read    { printf("READ\n"); }
rand    { printf("RAND\n"); }
dup     { printf("DUP\n"); }
op      { printf("OP\n"); }
halt    { printf("HALT\n"); }

"."[a-zA-Z_][a-zA-Z0-9_]* {
    printf("nice procedure (%s)\n", yytext + 1);
}

[a-zA-Z_][a-zA-Z0-9_]*":" {
    yytext[strlen(yytext) - 1] = 0;
    printf("nice label (%s)\n", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("nice identifier (%s)\n", yytext);
}

%%

int main(void) {
    yyin = fopen("../test.s", "r");
    yylex();
    return 0;
}
