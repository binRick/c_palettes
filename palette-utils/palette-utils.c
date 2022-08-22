#pragma once
#define INCBIN_SILENCE_BITCODE_WARNING
#ifndef PALETTE_UTILS_C
#define PALETTE_UTILS_C
#include "embeds/tbl1.c"
#include "palette-utils.h"

/////////////////////////////////////////////////////////////////////////////////////////////
#define MIN_PALETTE_SIZE                                       32
#define MAX_VALID_EMBEDDED_PALETTES_DATA_AGE_MINUTES           10
#define EMBEDDED_PALETTES_TABLE_HASH                           embedded_palettes_table_hash
#define EMBEDDED_PALETTE_NAMES_TABLE_HASH                      embedded_palettes_table_hash
/////////////////////////////////////////////////////////////////////////////////////////////
#define EMBEDDED_PALETTES_TABLE_ITEM                           embedded_palettes_table_item
#define EMBEDDED_PALETTES_TABLE_TIMESTAMP                      embedded_palettes_table_created_ts
#define EMBEDDED_PALETTES_TABLE_NAME                           embedded_palettes_table
#define EMBEDDED_PALETTES_TABLE_SIZE                           embedded_palettes_table_bytes
#define EMBEDDED_PALETTES_TABLE_QTY                            embedded_palettes_table_qty - 1
#define EMBEDDED_PALETTES_AGE_MS                               timestamp() - EMBEDDED_PALETTES_TABLE_TIMESTAMP
#define EMBEDDED_PALETTES_AGE_STRING                           milliseconds_to_string(EMBEDDED_PALETTES_AGE_MS)
#define DEFAULT_STYLE                                          AC_GREEN
/////////////////////////////////////////////////////////////////////////////////////////////
#define PALETTE_UTILS_EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG    false
#define VERBOSE_MODE                                           true
/////////////////////////////////////////////////////////////////////////////////////////////

struct djbhash_node *EMBEDDED_PALETTES_TABLE_ITEM;
struct djbhash      EMBEDDED_PALETTES_TABLE_HASH;

static char *__basename(const char *path){
  const char *slash = strrchr(path, '/');

  if (slash) {
    path = slash + 1;
  }
  return((char *)path);
}

int parse_embedded_palettes(){
  //ct_start(NULL);
  int         i = 0; EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  uint32_t    r;
  JSON_Value  *o;
  JSON_Object *O;
  short       ok;
  char        *LINE, *NAME, *HEX, *colp, *ptr, *js;
  int         qty = 0, ui, palettes_qty = 0;

  char        *palette_line;

  while (EMBEDDED_PALETTES_TABLE_ITEM && (++i < EMBEDDED_PALETTES_TABLE_QTY)) {
    char   *palette      = __basename((char *)((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename);
    size_t palette_size  = (size_t)((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->size;
    char   *palette_data = (char *)((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->data;
    if (strlen(palette_data) < MIN_PALETTE_SIZE) {
      continue;
    }
    struct StringFNStrings palette_lines = stringfn_split_lines_and_trim(palette_data);
    if (PALETTE_UTILS_EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG) {
      fprintf(stdout,
              AC_RESETALL AC_REVERSED AC_MAGENTA_BLACK "%s" AC_RESETALL
              AC_RESETALL AC_REVERSED AC_RED_BLACK AC_BOLD "%d" AC_RESETALL
              AC_RESETALL " " AC_REVERSED AC_GREEN_BLACK AC_BOLD "Lines" AC_RESETALL
              "\n",
              palette_data,
              palette_lines.count
              );
    }
    struct StringFNStrings palette_line_split;
    char                   *palette_line_key, *palette_line_val;
    char                   *json_string;
    for (int li = 0; li < palette_lines.count; li++) {
      palette_line = stringfn_trim(palette_lines.strings[li]);
      if (strlen(palette_line) < 1) {
        continue;
      }
      palette_line_split = stringfn_split(palette_line, '=');
      if (palette_line_split.count != 2) {
        continue;
      }
      palette_line_key = strdup(palette_line_split.strings[0]);
      palette_line_val = strdup(stringfn_to_uppercase(palette_line_split.strings[1]));
      if ((strlen(palette_line_key) < 5) || (strlen(palette_line_val) != 6)) {
        continue;
      }

      if (!stringfn_starts_with(palette_line_val, "#")) {
        HEX = malloc(strlen(palette_line_val) + 1);
        sprintf(HEX, "#%s", palette_line_val);
        palette_line_val = strdup(HEX);
        free(HEX);
      }

      if (strlen(palette_line_val) != 7) {
        continue;
      }

      ColorInfo *C = malloc(sizeof(ColorInfo));
      C->name = strdup(palette_line_key);
      free(palette_line_key);
      C->hex = strdup(palette_line_val);
      free(palette_line_val);
      r = rgba_from_string(C->hex, &ok);
      rgba_t rgba = rgba_new(r);
      ui = strtoul(C->hex, &ptr, 16);
      ////////////////////
      C->ansicode      = 0; //hex_to_closest_ansi_code(ui);
      C->rgb           = malloc(sizeof(RGB_t));
      C->ansi          = malloc(sizeof(Ansi_t));
      C->truecolor     = malloc(sizeof(Ansi_t));
      C->truecolor->fg = malloc(32);
      C->truecolor->bg = malloc(32);
      C->ansi->fg      = malloc(32);
      C->ansi->bg      = malloc(32);
      ////////////////////
      C->rgb->red   = (uint32_t)r >> 24 & 0xff;
      C->rgb->green = (uint32_t)r >> 16 & 0xff;
      C->rgb->blue  = (uint32_t)r >> 8 & 0xff;
      C->rgb->alpha = (uint32_t)r & 0xff;
      ////////////////////
      sprintf(C->truecolor->fg, TRUECOLOR_FG_CODE, C->rgb->red, C->rgb->green, C->rgb->blue);
      sprintf(C->truecolor->bg, TRUECOLOR_BG_CODE, C->rgb->red, C->rgb->green, C->rgb->blue);
      sprintf(C->ansi->fg, "\x1b[38;5;%dm", C->ansicode);
      sprintf(C->ansi->bg, "\x1b[48;5;%dm", C->ansicode);
      if (PALETTE_UTILS_EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG) {
        fprintf(stdout,
                AC_UNDERLINE AC_YELLOW_BLACK "RGB" AC_RESETALL
                "  =>  "
                AC_GREEN AC_REVERSED AC_ITALIC "[%s]" AC_RESETALL
                "  =>  "
                AC_BLUE AC_REVERSED AC_ITALIC "[%10s]" AC_RESETALL
                "  =>  "
                AC_MAGENTA AC_REVERSED AC_ITALIC "[%s]" AC_RESETALL
                "  =>  "
                AC_BLUE AC_REVERSED AC_ITALIC "[%3dx%3dx%3d]" AC_RESETALL
                "\n",
                palette,
                C->name,
                C->hex, C->rgb->red, C->rgb->green, C->rgb->blue
                );
      }
      o = json_value_init_object();
      O = json_value_get_object(o);
      json_object_set_string(O, "name", C->name);
      json_object_set_string(O, "hex", C->hex);
      json_object_dotset_number(O, "ansicode", C->ansicode);
      json_object_dotset_number(O, "rgb.red", C->rgb->red);
      json_object_dotset_number(O, "rgb.green", C->rgb->green);
      json_object_dotset_number(O, "rgb.blue", C->rgb->blue);
      json_object_dotset_string(O, "seq.ansi.fg", C->ansi->fg);
      json_object_dotset_string(O, "seq.ansi.bg", C->ansi->bg);
      json_object_dotset_string(O, "seq.truecolor.fg", C->truecolor->fg);
      json_object_dotset_string(O, "seq.truecolor.bg", C->truecolor->bg);
      qty++;
      json_string = json_serialize_to_string(o);
      //assert_nonnull(json_string);
      ////////////////////////////////////////////////////
      if (DEBUG_PALETTE_JSON_CONTENT || DEBUG_PALETTE_PRETTY_JSON_CONTENT) {
        if (DEBUG_PALETTE_CONTENT_COLORS) {
          ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
        }
        if (DEBUG_PALETTE_PRETTY_JSON_CONTENT) {
          free(json_string);
          json_string = json_serialize_to_string_pretty(o);
          //assert_nonnull(json_string);
        }
        if (DEBUG_PALETTE_JSON_CONTENT) {
          fprintf(stdout, "%s\n", json_string);
        }
        if (DEBUG_PALETTE_CONTENT_COLORS) {
          ansi_reset(stdout);
        }
        free(json_string);
      }
      ///////////////////////////////////////////////////////////////////////////////////////////
    }

    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
    palettes_qty++;
  }
  // ct_set_unit(ct_MILLISECONDS);
  char *parse_embedded_palettes_dur = "";

  //ct_stop("");

  if (VERBOSE_MODE) {
    fprintf(stderr,
            acs(DEFAULT_STYLE "Parsed ")
            acs(AC_GREEN_BLACK AC_BOLD AC_REVERSED "%d")
            acs(DEFAULT_STYLE " Palettes from ")
            acs(AC_GREEN_BLACK AC_BOLD AC_REVERSED "%d")
            acs(DEFAULT_STYLE " Palette Lines in ")
            acs(AC_GREEN_BLACK AC_ITALIC AC_REVERSED "%s")
            acs(DEFAULT_STYLE " from ")
            acs(AC_BLUE_BLACK AC_BOLD AC_REVERSED "%s")
            acs(DEFAULT_STYLE " embedded file containing ")
            acs(AC_GREEN_BLACK AC_BOLD AC_REVERSED "%lu")
            acs(DEFAULT_STYLE " items created ")
            acs(AC_BLUE_BLACK AC_ITALIC AC_REVERSED "%s")
            acs(DEFAULT_STYLE " ago.\n"),
            palettes_qty,
            qty,
            parse_embedded_palettes_dur,
            bytes_to_string(EMBEDDED_PALETTES_TABLE_SIZE),
            EMBEDDED_PALETTES_TABLE_QTY,
            EMBEDDED_PALETTES_AGE_STRING
            );
  }
  return(0);
} /* parse_embedded_palettes */

struct StringBuffer *strings_to_stringbuffer(struct StringFNStrings S){
  struct StringBuffer *sb = stringbuffer_new_with_options(1024, true);

  for (int i = 0; i < S.count; i++) {
    stringbuffer_append_string(sb, S.strings[i]);
    stringbuffer_append_string(sb, "\n");
  }
  return(sb);
}

struct StringFNStrings stringbuffer_to_strings(struct StringBuffer *SB){
  struct StringFNStrings items = stringfn_split_lines_and_trim(stringfn_trim(stringbuffer_to_string(SB)));

  return(items);
}

struct StringFNStrings get_palette_names(){
  struct StringBuffer    *sb = stringbuffer_new_with_options(1024, true);
  struct StringFNStrings S   = get_palette_files();

  for (int i = 0; i < S.count; i++) {
    if (strlen(stringfn_trim(S.strings[i])) < 1) {
      continue;
    }
    stringbuffer_append_string(sb, stringfn_trim(__basename(S.strings[i])));
    stringbuffer_append_string(sb, "\n");
  }
  return(stringbuffer_to_strings(sb));
}

struct StringFNStrings get_palette_files(){
  struct StringBuffer *sb = stringbuffer_new_with_options(1024, true);

  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  for (int i = 0; EMBEDDED_PALETTES_TABLE_ITEM; i++) {
    if (strlen(EMBEDDED_PALETTES_TABLE_NAME[i].filename) < 1) {
      continue;
    }
    stringbuffer_append_string(sb,
                               ((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename
                               );
    stringbuffer_append_string(sb, "\n");
    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  }
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  return(stringbuffer_to_strings(sb));
}

int load_palettes_hash(){
  djbhash_init(&EMBEDDED_PALETTES_TABLE_HASH);
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH)
  ;
  for (int i = 0; i < EMBEDDED_PALETTES_TABLE_QTY; i++) {
    if (strlen(EMBEDDED_PALETTES_TABLE_NAME[i].filename) < 1) {
      continue;
    }
    char *FILENAME = strdup(EMBEDDED_PALETTES_TABLE_NAME[i].filename);
    djbhash_set(
      &EMBEDDED_PALETTES_TABLE_HASH,
      FILENAME,
      &EMBEDDED_PALETTES_TABLE_NAME[i],
      DJBHASH_OTHER
      );
    free(FILENAME);
  }
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  fprintf(stderr, "Loaded %d Embedded Palettes\n", EMBEDDED_PALETTES_TABLE_HASH.active_count);
  return(0);
}

char *denormalize_palette_filename(char *FILENAME){
  if (!stringfn_starts_with(FILENAME, "./")) {
    char *FF = malloc(strlen(FILENAME) - 2);
    sprintf(FF, "./%s", FILENAME);
    return(FF);
  }
  return(FILENAME);
}

char *normalize_palette_filename(char *FILENAME){
  if (stringfn_starts_with(FILENAME, "./")) {
    char *FF = stringfn_substring(FILENAME, 2, 0);
    return(FF);
  }
  return(FILENAME);
}

bool palette_file_exists(char *PALETTE_FILE){
  return(
    (djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, PALETTE_FILE) != NULL)
    ||
    (djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, normalize_palette_filename(PALETTE_FILE)) != NULL)
    ||
    (djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, denormalize_palette_filename(PALETTE_FILE)) != NULL)
    );
}

char *prefix_string(char *PREFIX, char *STRING){
  struct StringBuffer *sb = stringbuffer_new_with_options(strlen(STRING) + 2, true);

  stringbuffer_append_string(sb, PREFIX);
  stringbuffer_append_string(sb, STRING);
  char *NS = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  return(NS);
}

int palette_name_exists_qty(char *PALETTE_NAME){
  int                    qty   = 0;
  struct StringFNStrings names = get_palette_names();

  for (int i = 0; i < names.count; i++) {
    if (stringfn_equal(PALETTE_NAME, names.strings[i])) {
      qty++;
    }
  }
  return(qty);
}

struct StringFNStrings palette_file_content(char *PALETTE_FILE){
  struct StringBuffer *sb = stringbuffer_new_with_options(1024, true);

  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, PALETTE_FILE);
  if (EMBEDDED_PALETTES_TABLE_ITEM != NULL) {
    stringbuffer_append_string(sb, stringfn_trim(((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->data));
  }else{
    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, prefix_string("./", PALETTE_FILE));
    if (EMBEDDED_PALETTES_TABLE_ITEM != NULL) {
      stringbuffer_append_string(sb, stringfn_trim(((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->data));
    }
  }
  return(stringbuffer_to_strings(sb));
}

#endif
