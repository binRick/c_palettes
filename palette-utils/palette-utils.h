#pragma once
#ifndef PALETTE_UTILS_H
#define PALETTE_UTILS_H
#include <stdio.h>
#include <string.h>
#define DEBUG_PALETTE_CONTENT_COLORS             false
#define DEBUG_PALETTE_PRETTY_JSON_CONTENT        false
#define DEBUG_PALETTE_JSON_CONTENT               false
#define EMBEDDED_PALETTES_TABLE_DEBUG            false
#define EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG    false
#include "palettes/palettes.h"
#include "submodules/assertf/assertf.h"
#include "submodules/bytes/bytes.h"
#include "submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/c_stringfn/include/stringfn.h"
#include "submodules/c_timer/include//c_timer.h"
#include "submodules/djbhash/src/djbhash.h"
#include "submodules/fs.c/fs.h"
#include "submodules/generic-print/print.h"
#include "submodules/ms/ms.h"
#include "submodules/rgba/src/rgba.h"
#include "submodules/timestamp/timestamp.h"
#include "subprojects/parson/parson.h"
#include "vendor/ansicodes/rgbansi.h"
//////////////////////////////////////////////////////////////////////////////
int load_palettes_hash();
int parse_embedded_palettes();
struct StringFNStrings get_palette_files();
struct StringFNStrings get_palette_names();
int palette_name_exists_qty(char *PALETTE_NAME);
bool palette_file_exists(char *PALETTE_FILE);
struct StringFNStrings palette_file_content(char *PALETTE_FILE);
struct StringBuffer *strings_to_stringbuffer(struct StringFNStrings S);

//////////////////////////////////////////////////////////////////////////////

#endif
