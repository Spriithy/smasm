#ifndef COLORS_H
#define COLORS_H

#include <stdio.h>
#include <stdlib.h>

#define COLOR_RED "\x1b[1;31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_NONE "\x1b[0m"

#define ERROR(fmt, ...)                                         \
  printf(COLOR_RED "error: " COLOR_NONE fmt "\n", __VA_ARGS__); \
  exit(1)

#define LOG(s) printf(COLOR_BLUE "log: " COLOR_NONE s "\n");

#endif  // COLORS_H
