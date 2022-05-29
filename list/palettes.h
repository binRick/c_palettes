#pragma once
#ifndef PALETTES_LIST_H
#define PALETTES_LIST_H
#include "../embeds/tbl1.c"
#define MIN_PALETTE_SIZE    32
#include "../palette-utils/palette-utils.h"
#include "list.h"
////////////////////////////////////////////
//int load_palettes_hash();
int parse_embedded_palettes();
int parse_args(int, char **);
int debug_args();
int print_strings(struct StringFNStrings items);
int palette_name_exists_qty(char *PALETTE_NAME);

//////////////////////////////////////////////////////////////////////////
char *prefix_string(char *PREFIX, char *STRING);
char *normalize_palette_filename(char *FILENAME);
char *denormalize_palette_filename(char *FILENAME);

//////////////////////////////////////////////////////////////////////////
bool palette_name_exists(char *PALETTE_NAME);
bool palette_file_exists(char *PALETTE_FILE);

//////////////////////////////////////////////////////////////////////////
struct StringBuffer *strings_to_stringbuffer(struct StringFNStrings);

//////////////////////////////////////////////////////////////////////////
struct StringFNStrings get_palette_names();
struct StringFNStrings palette_file_content(char *PALETTE_FILE);
struct StringFNStrings palette_name_files(char *PALETTE_NAME);
struct StringFNStrings get_palette_file_names();
struct StringFNStrings get_palette_files();
struct StringFNStrings get_palette_colors();
struct StringFNStrings stringbuffer_to_strings(struct StringBuffer *SB);

#include "debug.h"
#include "palettes.c"
#include "utils.c"
/////////////////////////////////////////////////////////////////////////////////////////////
#endif
