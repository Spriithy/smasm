#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "asm/compiler.h"
#include "colors.h"
#include "vm/interpreter.h"
#include "vm/loader.h"

void usage() {
  printf(
      "usage:    smasm <flags> <input> [output]\n"
      "\tl       will generate a .log file reporting everything happened\n"
      "\te       is used to execute a file\n"
      "\tc       will compile a source file\n");
}

int main(int argc, char **argv) {
  srand(time(NULL));

  int flagC = 0, flagE = 0, flagL = 0;

  char *in, *out = "a.out";

  switch (argc) {
    case 4:
      out = argv[3];
    case 3: {
      if (argv[1][0] != '-') goto error;
      char c = argv[1][1];
      while ((c = *argv[1]++)) switch (c) {
          case 'e':
            flagE++;
            break;
          case 'c':
            flagC++;
            break;
          case 'l':
            flagL++;
            break;
          case '-':
            break;
          default:
            printf("unrecognized flag '%c'\n", c);
            goto error;
        }
      in = argv[2];
    } break;
    default:
      goto error;
  }

  if (flagC) {
    // Compile
    compile(in, out, flagL);
    in = out;
  }

  if (flagE) {
    // Execute
    opcode *ops = load_file(in);
    return execute(ops);
  }

  return 0;

error:
  usage();
  exit(1);
}
