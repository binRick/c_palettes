#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "parson/parson.h"
#include "submodules/bytes/bytes.h"
#include "submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/c_ansi/ansi-rgb-utils/ansi-rgb-utils.h"
#include "submodules/c_ansi/ansi-utils/ansi-utils.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/c_stringfn/include/stringfn.h"
#include "submodules/cargs/include/cargs.h"
#include "submodules/dbg/dbg.h"
#include "submodules/djbhash/src/djbhash.h"
#include "submodules/fs.c/fs.h"
#include "submodules/generic-print/print.h"
#include "submodules/ms/ms.h"
#include "submodules/rgba/src/rgba.h"
#include "submodules/timestamp/timestamp.h"

#define RENDER_COLOR_OUTPUT     false
#define RENDER_PRETTY_JSON      true
#define DEFAULT_COUNT           10
#define DEFAULT_VERBOSE         false
#define DEFAULT_JSON_MODE       false
#define DEFAULT_PRETTY          false
#define DEFAULT_COLOR           false
#define DEFAULT_MODE            "names"
#define DEFAULT_PALETTE_NAME    "vscode"
#define DEFAULT_PALETTE_FILE    "./kfc/dark/vscode"
#define C256_FG_CODE            "\x1b[38;5;%dm"
#define C256_BG_CODE            "\x1b[48;5;%dm"
#define TRUECOLOR_FG_CODE       "\x1b[38;2;%d;%d;%dm"
#define TRUECOLOR_BG_CODE       "\x1b[48;2;%d;%d;%dm"
#define RESET_CODE              "\x1b[0m"

int parse_args(int argc, char *argv[]);
int debug_args();
int hex_to_closest_ansi_code(const uint32_t trp);
int list_names();

typedef struct CLI_ARGS {
  char *mode;
  bool verbose;
  int  count;
  bool pretty;
  int  color;
  char *palette;
  char *file;
} args_t;

static struct cag_option options[] = {
  { .identifier     = 'm',
    .access_letters = "m",
    .access_name    = "mode",
    .value_name     = "MODE",
    .description    = "Mode" },
  { .identifier     = 'v',
    .access_letters = "v",
    .access_name    = "verbose",
    .value_name     = NULL,
    .description    = "Verbose Mode" },
  { .identifier     = 'c',
    .access_letters = "c",
    .access_name    = "count",
    .value_name     = "COUNT",
    .description    = "Item Count" },
  { .identifier     = 'P',
    .access_letters = "P",
    .access_name    = "pretty",
    .value_name     = NULL,
    .description    = "JSON Pretty Mode" },
  { .identifier     = 'x',
    .access_letters = "C",
    .access_name    = "color",
    .value_name     = NULL,
    .description    = "Color Mode" },
  { .identifier     = 'p',
    .access_letters = "p",
    .access_name    = "palette",
    .value_name     = "PALETTE",
    .description    = "Palette" },
  { .identifier     = 'f',
    .access_letters = "f",
    .access_name    = "file",
    .value_name     = "FILE",
    .description    = "Palette File" },
  { .identifier     = 'h',
    .access_letters = "h",
    .access_name    = "help",
    .description    = "Shows the command help" }
};
