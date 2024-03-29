#pragma once
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#define PALETTES_DB_PATH    "etc/palettes.sqlite"
#include "db/db.h"
#include "palette-utils/palette-utils.h"
#include "palettes/palettes.h"
#include "submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/c_fsio/include/fsio.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/rgba/src/rgba.h"
#include "subprojects/cargs/include/cargs.h"

////////////////////////////////////////////////////////////////////
#define PARSED_COLORS_JSON_FILE    "etc/parsed-colors.json"
#define DEFAULT_INPUT              PARSED_COLORS_JSON_FILE
#define DEFAULT_OUTPUT_DIR         "etc/images"
#define DEFAULT_COUNT              5
#define DEFAULT_VERBOSE            false
#define DEFAULT_MODE               "write"
////////////////////////////////////////////////////////////////////

typedef struct CLI_ARGS {
  char *input;
  char *output_dir;
  char *mode;
  bool verbose;
  int  count;
} args_t;

static struct cag_option options[] = {
  { .identifier     = 'i',
    .access_letters = "i",
    .access_name    = "input",
    .value_name     = "INPUT",
    .description    = "Input File Path" },
  { .identifier     = 'o',
    .access_letters = "o",
    .access_name    = "output_dir",
    .value_name     = "OUTPUT_DIR",
    .description    = "Output Directory" },
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
  { .identifier     = 'h',
    .access_letters = "h",
    .access_name    = "help",
    .description    = "Shows the command help" }
};
