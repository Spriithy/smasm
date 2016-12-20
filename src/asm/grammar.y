%{

#include <stdio.h>
#include "ast.h"
#include "compiler.h"
#include "symtable.h"
#include "../instructions.h"
#include "../colors.h"

extern int yylex(void);

extern int yylineno;

int yydolog = 0;

extern FILE *yyin, *yyout;
FILE *yylog;

S_TABLE *labels;
S_TABLE *procedures;
node *ast_head, *ast_tail;

int pc = 0;
int errcount = 0;

char *tok;

%}

%union {
    int  num;
    char *ident;
}

%token <num> INT
%token <ident> IDENT TOKEN_LBL TOKEN_PROC
%token <ident> TOKEN_POP TOKEN_IPOP TOKEN_PUSH TOKEN_IPUSH TOKEN_PUSHS
%token <ident> TOKEN_CALL TOKEN_RET TOKEN_RETZ TOKEN_RETNZ TOKEN_JPC TOKEN_JMP
%token <ident> TOKEN_READ TOKEN_WRITE TOKEN_MSWAP
%token <ident> TOKEN_RAND TOKEN_DUP TOKEN_OP TOKEN_HALT

%token NL

%%

program
    : %empty
    | stat NL program
    | TOKEN_LBL {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: label -> %s\n", pc, $1);
        add_sym_entry(labels, $1, pc);
    } stat NL program
    | TOKEN_PROC {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: procedure -> %s\n", pc, $1);
        add_sym_entry(procedures, $1, pc);
    } NL program
    | NL program
    ;

stat
    : TOKEN_POP INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: pop %d\n", pc, $2);
        ast_tail->next = new_instr(POP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSH INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: push %d\n", pc, $2);
        ast_tail->next = new_instr(PUSH, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_PUSHS INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: push %d\n", pc, $2);
        ast_tail->next = new_instr(PUSHS, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPOP {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: ipop\n", pc);
        ast_tail->next = new_instr(IPOP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_IPUSH {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: ipush\n", pc);
        ast_tail->next = new_instr(IPUSH, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_CALL IDENT {
        tok = $2;
        if (yydolog) fprintf(yylog, "%3d: call %s\n", pc, $2);
        ast_tail->next = new_ctrl(CALL, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RET {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: ret\n", pc);
        ast_tail->next = new_instr(RET, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RETNZ {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: retnz\n", pc);
        ast_tail->next = new_instr(RETNZ, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RETZ {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: retz\n", pc);
        ast_tail->next = new_instr(RETZ, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JMP IDENT {
        tok = $2;
        if (yydolog) fprintf(yylog, "%3d: jmp %s\n", pc, $2);
        ast_tail->next = new_ctrl(JMP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_JPC IDENT {
        tok = $2;
        if (yydolog) fprintf(yylog, "%3d: jpc %s\n", pc, $2);
        ast_tail->next = new_ctrl(JPC, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_WRITE INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: write %d\n", pc, $2);
        ast_tail->next = new_instr(WRITE, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_READ INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: read %d\n", pc, $2);
        ast_tail->next = new_instr(READ, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_RAND INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: rand %d\n", pc, $2);
        ast_tail->next = new_instr(RND, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_DUP {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: dup\n", pc);
        ast_tail->next = new_instr(DUP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_MSWAP {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: mswap\n", pc);
        ast_tail->next = new_instr(MSWAP, 0, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_OP INT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: op %d\n", pc, $2);
        ast_tail->next = new_instr(OP, $2, pc++);
        ast_tail = ast_tail->next;
    }
    | TOKEN_HALT {
        tok = $1;
        if (yydolog) fprintf(yylog, "%3d: halt\n", pc);
        ast_tail->next = new_instr(HALT, 0, pc++);
        ast_tail = ast_tail->next;
    }
    ;

%%

int compile(char *in, char *out, int log) {
  yydolog = log;

  yyin = fopen(in, "r");
  yyout = fopen(out, "w+");
  if (log) yylog = fopen("asm.comp.log", "w+");

  if (yyin == NULL) { ERROR("couldn't open/read source file '%s'", in); }
  if (yyout == NULL) { ERROR("couldn't create/open file '%s'", out); }
  if (log) if (yylog == NULL) { ERROR("%s", "couldn't create/open file 'asm.comp.log'"); }

  ast_head = new_ph(VOID, "<start>", 0);
  ast_tail = ast_head;
  procedures = new_sym_table();
  labels = new_sym_table();

  do {
    yyparse();
  } while (!feof(yyin));

  node *p = ast_head;
  while ((p = p->next) != NULL) {
    errcount += emit(p, labels, procedures, yyout);
  }

  if (errcount) {
    printf("encountered " COLOR_RED "%d" COLOR_NONE " error%s\n", errcount,
           (errcount > 1 ? "s" : ""));
    remove(out);
  }

  if (log) {
    fprintf(yylog, "---[ Symbol tables ]---\n");
    fprintf(yylog, "   PROCEDURES:\n");
    for (int i = 0; i < procedures->size; i++)
      fprintf(yylog, "     %s : %d\n", procedures->syms[i], procedures->ofs[i]);

    fprintf(yylog, "   LABELS:\n");
    for (int i = 0; i < labels->size; i++)
      fprintf(yylog, "     %s : %d\n", labels->syms[i], labels->ofs[i]);
  }

  fclose(yyin);
  fclose(yyout);
  if (log) fclose(yylog);

  return 0;
}

yyerror(const char *s) {
    errcount++;
    printf(COLOR_RED "error:" COLOR_YELLOW "%d:" COLOR_NONE " %s. Unexpected token after '"
        COLOR_GREEN "%s" COLOR_NONE "'\n", yylineno, s, tok);
    return 1;
}
