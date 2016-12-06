#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/colors.h"
#include "include/compiler.h"

#define ASM_NAME "smasm"

void usage() {
  printf("usage:\n  " COLOR_GREEN "%s" COLOR_NONE "\t-[" COLOR_YELLOW
         "h" COLOR_NONE "|" COLOR_YELLOW "c" COLOR_NONE "|" COLOR_YELLOW
         "e" COLOR_NONE "] <" COLOR_BLUE "files" COLOR_NONE ">\n\n",
         ASM_NAME);
  printf(COLOR_YELLOW "  -h\t" COLOR_NONE " for some help\n");
  printf(COLOR_YELLOW "  -c\t" COLOR_NONE " to compile all given files\n");
  printf(COLOR_YELLOW "  -e\t" COLOR_NONE " to execute the given files\n");
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  printf("%d\n", argc);
  for (int i = 0; i < argc; i++) puts(argv[i]);

  switch (argc) {
    case 1:
      goto error;
    default:
      if (strlen(argv[1]) != 2 || argv[1][0] != '-')
        ERROR("unrecognized option: " COLOR_RED "%s\n" COLOR_NONE, argv[1]);

      switch (argv[1][1]) {
        case 'h':
          // Show help
          usage();
          break;
        case 'e':
          // Execute input files
          if (argc - 2 <= 0) ERROR("no input files%s\n", "");

          puts("Executing file.");

          break;
        case 'c':
          // Parse and compile input files
          if (argc - 2 <= 0) ERROR("no input files%s\n", "");

          puts("Compiling file.");

          compile(argv[2], 1);

          break;
        default:
          ERROR("unrecognized option: %s\n", argv[1]);
      }
  }

  return 0;

error:
  usage();
  exit(1);
}
