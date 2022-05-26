#pragma once
#include "list.h"
#include <string.h>


void ansi_reset(FILE *file) {
  fputs(RESET_CODE, file);
}


void ansi_256_fg(FILE *file, int color) {
  fprintf(file, C256_FG_CODE, color);
}


void ansi_256_bg(FILE *file, int color) {
  fprintf(file, C256_BG_CODE, color);
}


void ansi_truecolor_fg(FILE *file, int r, int g, int b) {
  fprintf(file, TRUECOLOR_FG_CODE, r, g, b);
}


void ansi_truecolor_bg(FILE *file, int r, int g, int b) {
  fprintf(file, TRUECOLOR_BG_CODE, r, g, b);
}


static char *__basename(const char *path){
  const char *slash = strrchr(path, '/');

  if (slash) {
    path = slash + 1;
  }
  return((char *)path);
}
