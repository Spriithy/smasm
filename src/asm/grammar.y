%{

%}

%token NUM ID
%token LBL PROC
%token POP IPOP PUSH IPUSH PUSHS
%token CALL RET JPC JMP
%token READ WRITE
%token RAND DUP OP HALT

%token NL

%%

program
    : PROC NL stat_list { printf("program\n"); }
    ;

stat_list
    : stat NL
    | stat_list stat NL
    ;

stat
    : POP INT { printf("pop int\n"); }
    ;

%%

int main(void) {
    do {
        yyparse();
    } while(!feof(yyin));
    return 0;
}