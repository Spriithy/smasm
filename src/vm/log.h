#ifndef LOG_H
#define LOG_H

#include "../colors.h"

#define VM_ERROR(fmt, ...)                                         \
  printf(COLOR_RED "vm-error: " COLOR_NONE fmt "\n", __VA_ARGS__); \
  exit(1)

#endif  // LOG_H
