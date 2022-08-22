/////////////////////////////////////////////////////////////////////////
#include "palettes/palettes.h"
/////////////////////////////////////////////////////////////////////////
#include "submodules/c_deps/submodules/b64.c/b64.h"
#include "submodules/c_deps/submodules/c_timer/include/c_timer.h"
#include "submodules/c_deps/submodules/cargs/include/cargs.h"
#include "submodules/c_deps/submodules/dbg/dbg.h"
#include "submodules/c_deps/submodules/generic-print/print.h"
#include "submodules/c_deps/submodules/incbin/incbin.h"
#include "submodules/c_deps/submodules/libyaml/include/yaml.h"
#include "submodules/c_deps/submodules/ms/ms.h"
#include "submodules/c_deps/submodules/parson/parson.h"
#include "submodules/c_deps/submodules/rgba/src/rgba.h"
#include "submodules/c_deps/submodules/timestamp/timestamp.h"
#include "submodules/c_deps/submodules/wildcardcmp/wildcardcmp.h"
/////////////////////////////////////////////////////////////////////////
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
////////////////////////////////////////////////////
#define DEFAULT_COUNT               10
#define DEFAULT_VERBOSE             false
#define DEFAULT_MODE                "args"
#define DEFAULT_PALETTE             "*"
#define DEFAULT_PALETTE_TYPE        "*"
#define DEFAULT_PALETTE_PROPERTY    "*"
#define DEFAULT_PRETTY_MODE         false
////////////////////////////////////////////////////
////////////////////////////////////////////////////
int parse_args(int argc, char *argv[]);
int debug_args();

////////////////////////////////////////////////////
typedef struct CLI_ARGS {
  char *mode;
  bool verbose;
  int  count;
  char *palette;
  char *palette_type;
  char *palette_property;
  bool pretty;
} args_t;
////////////////////////////////////////////////////
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
  { .identifier     = 't',
    .access_letters = "t",
    .access_name    = "type",
    .value_name     = "PALETTE_TYPE",
    .description    = "Palette Type" },
  { .identifier     = 'p',
    .access_letters = "p",
    .access_name    = "palette",
    .value_name     = "PALETTE",
    .description    = "Palette Name" },
  { .identifier     = 'P',
    .access_letters = "P",
    .access_name    = "property",
    .value_name     = "PROPERTY",
    .description    = "Palette Property Name" },
  { .identifier     = 'z',
    .access_letters = "z",
    .access_name    = "pretty",
    .value_name     = NULL,
    .description    = "Pretty Mode" },
  { .identifier     = 'h',
    .access_letters = "h",
    .access_name    = "help",
    .description    = "Shows the command help" }
};
////////////////////////////////////////////////////
