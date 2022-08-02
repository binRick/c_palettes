#pragma once
#include "palette-utils/palette-utils.h"
#include "submodules/c_deps/submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/c_deps/submodules/c_colors/hex-png-pixel-utils/hex-png-pixel-utils.h"
#include "submodules/c_deps/submodules/c_fsio/include/fsio.h"
#include "submodules/c_deps/submodules/incbin/incbin.h"
#include "submodules/c_deps/submodules/parson/parson.h"
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
///////////////////////////////////////////////////
///////////////////////////////////////////////////
const char *palettes_basename(const char *path);
struct Palette get_palette(char *PALETTE_DATA);

///////////////////////////////////////////////////
static const char *PALETTE_TYPE_NAMES[] = {
  "paleta",
  "kfc",
  "kitty",
};

static const int  PALETTE_TYPE_NAMES_QTY = sizeof(PALETTE_TYPE_NAMES) / sizeof(*PALETTE_TYPE_NAMES);

#ifdef HEX_LEN
#undef HEX_LEN
#endif
#define HEX_LEN       7
#define JSON_BYTES    1024 * 32
typedef char                HexColor;
typedef struct AnsiColors   AnsiColors;
typedef struct Palette      Palette;

struct AnsiColors {
  HexColor black[HEX_LEN], red[HEX_LEN], green[HEX_LEN], yellow[HEX_LEN], blue[HEX_LEN], magenta[HEX_LEN], cyan[HEX_LEN], white[HEX_LEN];
};

enum PALETTE_TYPES {
  PALETTE_TYPE_PALETA,
  PALETTE_TYPE_KFC,
  PALETTE_TYPE_KITTY,
};

struct Palette {
  char       Name[32];
  bool       Parsed, Valid;
  int        Type, Properties;
  char       *TypeName;
  AnsiColors *fgColors, *bgColors;
  HexColor   bgSelection[HEX_LEN], fgSelection[HEX_LEN];
  HexColor   colors[HEX_LEN], bright[HEX_LEN], cursor[HEX_LEN], border[HEX_LEN], bg[HEX_LEN], fg[HEX_LEN];
  char       JSON[JSON_BYTES], JSON_PRETTY[JSON_BYTES];
};

char *          get_palette_property_value(const char *PROPERTY_NAME, const char *PALETTE_DATA);
struct Palette  get_palette(char *PALETTE_DATA);
int             get_palette_data_type(char *PALETTE_DATA);
char *          get_palette_data_type_name(char *PALETTE_DATA);

#define APPLY_PALETTE_COLORS    "printf \"\
\\033]4;0;#$(echo $color00)\\033\\ \
\\033]4;1;#$(echo $color01)\\033\\ \
\\033]4;2;#$(echo $color02)\\033\\ \
\\033]4;3;#$(echo $color03)\\033\\ \
\\033]4;4;#$(echo $color04)\\033\\ \
\\033]4;5;#$(echo $color05)\\033\\ \
\\033]4;6;#$(echo $color06)\\033\\ \
\\033]4;7;#$(echo $color07)\\033\\ \
\\033]4;8;#$(echo $color08)\\033\\ \
\\033]4;9;#$(echo $color09)\\033\\ \
\\033]4;10;#$(echo $color10)\\033\\ \
\\033]4;11;#$(echo $color11)\\033\\ \
\\033]4;12;#$(echo $color12)\\033\\ \
\\033]4;13;#$(echo $color13)\\033\\ \
\\033]4;14;#$(echo $color14)\\033\\ \
\\033]4;15;#$(echo $color15)\\033\\ \
\\033]10;#$(echo $foreground)\\033\\ \
\\033]11;#$(echo $background)\\033\\ \
\\033]12;#$(echo $cursor)\\033\\ \
\\033[21D\""


#define PRINT_PALETTE_COLORS    "\
\033[48;5;0m  \033[0m\
\033[48;5;1m  \033[0m\
\033[48;5;2m  \033[0m\
\033[48;5;3m  \033[0m\
\033[48;5;4m  \033[0m\
\033[48;5;5m  \033[0m\
\033[48;5;6m  \033[0m\
\033[48;5;7m  \033[0m\n\
\033[48;5;8m  \033[0m\
\033[48;5;9m  \033[0m\
\033[48;5;10m  \033[0m\
\033[48;5;11m  \033[0m\
\033[48;5;12m  \033[0m\
\033[48;5;13m  \033[0m\
\033[48;5;14m  \033[0m\
\033[48;5;15m  \033[0m\n"
