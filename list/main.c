#include "palettes.h"
/////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_VALID_EMBEDDED_PALETTES_DATA_AGE_MINUTES    10
#define EMBEDDED_PALETTES_TABLE_HASH                    embedded_palettes_table_hash
#define EMBEDDED_PALETTE_NAMES_TABLE_HASH               embedded_palettes_table_hash
/////////////////////////////////////////////////////////////////////////////////////////////
#define EMBEDDED_PALETTES_TABLE_ITEM                    embedded_palettes_table_item
#define EMBEDDED_PALETTES_TABLE_TIMESTAMP               embedded_palettes_table_created_ts
#define EMBEDDED_PALETTES_TABLE_NAME                    embedded_palettes_table
#define EMBEDDED_PALETTES_TABLE_SIZE                    embedded_palettes_table_bytes
#define EMBEDDED_PALETTES_TABLE_QTY                     embedded_palettes_table_qty - 1
#define EMBEDDED_PALETTES_AGE_MS                        timestamp() - EMBEDDED_PALETTES_TABLE_TIMESTAMP
#define EMBEDDED_PALETTES_AGE_STRING                    milliseconds_to_string(EMBEDDED_PALETTES_AGE_MS)
#define DEFAULT_STYLE                                   AC_GREEN
#define DEBUG_ARGS                                      false
#define DEBUG_PARSED_ARGS                               false
/////////////////////////////////////////////////////////////////////////////////////////////
#include "../palette-utils/palette-utils.h"
//struct djbhash_node *EMBEDDED_PALETTES_TABLE_ITEM;
//struct djbhash      EMBEDDED_PALETTES_TABLE_HASH;
args_t args = {
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
  DEFAULT_PRETTY,
  DEFAULT_COLOR,
  DEFAULT_PALETTE_NAME,
  DEFAULT_PALETTE_FILE,
};

/////////////////////////////////////////////////////////////////////////////////////////////
int parse_embedded_palettes(){
  ct_start(NULL);
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
    if (strlen(palette_data) < MIN_PALETTE_SIZE)
      continue;
    struct StringFNStrings palette_lines = stringfn_split_lines_and_trim(palette_data);
    if (EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG)
      fprintf(stdout,
              AC_RESETALL AC_REVERSED AC_MAGENTA_BLACK "%s" AC_RESETALL
              AC_RESETALL AC_REVERSED AC_RED_BLACK AC_BOLD "%d" AC_RESETALL
              AC_RESETALL " " AC_REVERSED AC_GREEN_BLACK AC_BOLD "Lines" AC_RESETALL
              "\n",
              palette_data,
              palette_lines.count
              );
    struct StringFNStrings palette_line_split;
    char                   *palette_line_key, *palette_line_val;
    char                   *json_string;
    for (int li = 0; li < palette_lines.count; li++) {
      palette_line = stringfn_trim(palette_lines.strings[li]);
      if (strlen(palette_line) < 1)
        continue;
      palette_line_split = stringfn_split(palette_line, '=');
      if (palette_line_split.count != 2)
        continue;
      palette_line_key = strdup(palette_line_split.strings[0]);
      palette_line_val = strdup(stringfn_to_uppercase(palette_line_split.strings[1]));
      if ((strlen(palette_line_key) < 5) || (strlen(palette_line_val) != 6))
        continue;

      if (!stringfn_starts_with(palette_line_val, "#")) {
        HEX = malloc(strlen(palette_line_val) + 1);
        sprintf(HEX, "#%s", palette_line_val);
        palette_line_val = strdup(HEX);
        free(HEX);
      }

      if (strlen(palette_line_val) != 7)
        continue;

      ColorInfo *C = malloc(sizeof(ColorInfo));
      C->name = strdup(palette_line_key);
      free(palette_line_key);
      C->hex = strdup(palette_line_val);
      free(palette_line_val);
      r = rgba_from_string(C->hex, &ok);
      rgba_t rgba = rgba_new(r);
      ui = strtoul(C->hex, &ptr, 16);
      ////////////////////
      C->ansicode      = hex_to_closest_ansi_code(ui);
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
      if (EMBEDDED_PALETTES_TABLE_DEBUG)
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
      assert_nonnull(json_string);
      ////////////////////////////////////////////////////
      if (DEBUG_PALETTE_JSON_CONTENT || DEBUG_PALETTE_PRETTY_JSON_CONTENT) {
        if (DEBUG_PALETTE_CONTENT_COLORS)
          ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
        if (DEBUG_PALETTE_PRETTY_JSON_CONTENT) {
          free(json_string);
          json_string = json_serialize_to_string_pretty(o);
          assert_nonnull(json_string);
        }
        if (DEBUG_PALETTE_JSON_CONTENT)
          fprintf(stdout, "%s\n", json_string);
        if (DEBUG_PALETTE_CONTENT_COLORS)
          ansi_reset(stdout);
        free(json_string);
      }
      ///////////////////////////////////////////////////////////////////////////////////////////
    }

    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
    palettes_qty++;
  }
  ct_set_unit(ct_MILLISECONDS);
  char *parse_embedded_palettes_dur = ct_stop("");

  if (args.verbose)
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
  return(0);
} /* parse_embedded_palettes */

int debug_args(){
  fprintf(stderr,
          acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d")
          ", "
          ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Mode: %s")
          ", "
          ansistr(AC_RESETALL AC_UNDERLINE "Count: %d")
          "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Pretty Mode: %d")
          "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Color Mode: %d")
          "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Palette: %s")
          "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "File: %s")
          "\n",
          args.verbose, args.mode, args.count,
          args.pretty, args.color,
          args.palette, args.file
          );
  return(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  int r;

  r = parse_args(argc, argv);
  assert_eq(r, 0, %d);

  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }

  if ((strcmp(args.mode, "debug_args") == 0))
    return(debug_args());

  if ((strcmp(args.mode, "load") == 0))
    return(load_palettes_hash());

  if ((strcmp(args.mode, "parse") == 0)) {
    r = load_palettes_hash();
    assert_eq(r, 0, %d);
    return(parse_embedded_palettes());
  }

  r = load_palettes_hash();
  assert_eq(r, 0, %d);
  r = parse_embedded_palettes();
  assert_eq(r, 0, %d);

  if ((strcmp(args.mode, "palette-files") == 0)) {
    struct StringFNStrings items = get_palette_files();
    if (args.verbose)
      print_strings(items);
    printf(AC_RESETALL AC_GREEN AC_REVERSED "%d palette files.\n" AC_RESETALL, items.count);
    return(0);
  }

  if ((strcmp(args.mode, "palette-names") == 0)) {
    struct StringFNStrings items = get_palette_names();
    if (args.verbose)
      print_strings(items);
    printf(AC_RESETALL AC_GREEN AC_REVERSED "%d palette names.\n" AC_RESETALL, items.count);
    return(0);
  }

  if ((strcmp(args.mode, "palette-file-exists") == 0))
    return(palette_file_exists(args.file) ? 0 : 1);


  if ((strcmp(args.mode, "palette-name-exists") == 0))
    return(palette_name_exists(args.palette) ? 0 : 1);


  if ((strcmp(args.mode, "palette-name-exists-qty") == 0)) {
    printf("%d\n", palette_name_exists_qty(args.palette));
    return(0);
  }

  if ((strcmp(args.mode, "palette-name-files") == 0)) {
    print_strings(palette_name_files(args.palette));
    return(0);
  }

  if ((strcmp(args.mode, "palette-file-content") == 0)) {
    print_strings(palette_file_content(args.file));
    return(0);
  }

  if ((strcmp(args.mode, "colors") == 0)) {
    struct StringFNStrings items = get_palette_colors(args.palette);
    if (args.verbose)
      print_strings(items);
    printf(AC_RESETALL AC_GREEN AC_REVERSED "%d palette names.\n" AC_RESETALL, items.count);
    return(0);
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);

  return(EXIT_SUCCESS);
} /* main */

char *prefix_string(char *PREFIX, char *STRING){
  struct StringBuffer *sb = stringbuffer_new_with_options(strlen(STRING) + 2, true);

  stringbuffer_append_string(sb, PREFIX);
  stringbuffer_append_string(sb, STRING);
  char *NS = stringbuffer_to_string(sb);

  stringbuffer_release(sb);
  return(NS);
}

struct StringFNStrings palette_name_files(char *PALETTE_NAME){
  struct StringBuffer    *sb   = stringbuffer_new_with_options(1024, true);
  struct StringFNStrings files = get_palette_files();

  for (int i = 0; i < files.count; i++)
    if (
      stringfn_equal(PALETTE_NAME, files.strings[i])
      ||
      stringfn_ends_with(files.strings[i], prefix_string("/", PALETTE_NAME))
      ) {
      stringbuffer_append_string(sb, files.strings[i]);
      stringbuffer_append_string(sb, "\n");
    }
  return(stringbuffer_to_strings(sb));
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

int palette_name_exists_qty(char *PALETTE_NAME){
  int                    qty   = 0;
  struct StringFNStrings names = get_palette_names();

  for (int i = 0; i < names.count; i++)
    if (stringfn_equal(PALETTE_NAME, names.strings[i]))
      qty++;
  return(qty);
}

bool palette_name_exists(char *PALETTE_NAME){
  return(palette_name_exists_qty(PALETTE_NAME) > 0);
}

struct StringFNStrings get_palette_colors(char *PALETTE_NAME){
  struct StringBuffer    *sb   = stringbuffer_new_with_options(1024, true);
  struct StringFNStrings files = get_palette_files();
  struct StringFNStrings names = get_palette_names();
  struct StringFNStrings items = stringfn_split_lines_and_trim(stringbuffer_to_string(sb));

  stringbuffer_release(sb);
  return(items);
}

int print_strings(struct StringFNStrings S){
  for (int i = 0; i < S.count; i++)
    printf("%s\n", S.strings[i]);
  return(0);
}

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
    if (strlen(stringfn_trim(S.strings[i])) < 1)
      continue;
    stringbuffer_append_string(sb, stringfn_trim(__basename(S.strings[i])));
    stringbuffer_append_string(sb, "\n");
  }
  return(stringbuffer_to_strings(sb));
}

struct StringFNStrings palette_file_content(char *PALETTE_FILE){
  struct StringBuffer *sb = stringbuffer_new_with_options(1024, true);

  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, PALETTE_FILE);
  if (EMBEDDED_PALETTES_TABLE_ITEM != NULL)
    stringbuffer_append_string(sb, stringfn_trim(((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->data));
  else{
    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_find(&EMBEDDED_PALETTES_TABLE_HASH, prefix_string("./", PALETTE_FILE));
    if (EMBEDDED_PALETTES_TABLE_ITEM != NULL)
      stringbuffer_append_string(sb, stringfn_trim(((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->data));
  }
  return(stringbuffer_to_strings(sb));
}

struct StringFNStrings get_palette_files(){
  struct StringBuffer *sb = stringbuffer_new_with_options(1024, true);

  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  for (int i = 0; EMBEDDED_PALETTES_TABLE_ITEM; i++) {
    if (strlen(EMBEDDED_PALETTES_TABLE_NAME[i].filename) < 1)
      continue;
    stringbuffer_append_string(sb,
                               ((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename
                               );
    stringbuffer_append_string(sb, "\n");
    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  }
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  return(stringbuffer_to_strings(sb));
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

/*
 * int load_palettes_hash(){
 * djbhash_init(&EMBEDDED_PALETTES_TABLE_HASH);
 * djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
 * EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
 * char *FILENAME;
 *
 * for (int i = 0; i < EMBEDDED_PALETTES_TABLE_QTY; i++) {
 *  if (strlen(EMBEDDED_PALETTES_TABLE_NAME[i].filename) < 1) {
 *    continue;
 *  }
 *  FILENAME = strdup(EMBEDDED_PALETTES_TABLE_NAME[i].filename);
 *  djbhash_set(
 *    &EMBEDDED_PALETTES_TABLE_HASH,
 *    FILENAME,
 *    &EMBEDDED_PALETTES_TABLE_NAME[i],
 *    DJBHASH_OTHER
 *    );
 *  free(FILENAME);
 * }
 *
 *
 * djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
 * EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
 * for (int i = 0; EMBEDDED_PALETTES_TABLE_ITEM; i++) {
 *  if (EMBEDDED_PALETTES_TABLE_DEBUG) {
 *    char *msg = malloc(4096);
 *    sprintf(msg,
 *            AC_YELLOW_BLACK AC_REVERSED "#%d" AC_RESETALL
 *            "> "
 *            "|"
 *            "size:"
 *            AC_BLUE_BLACK AC_REVERSED "%lu" AC_RESETALL
 *            "|"
 *            "filename:"
 *            AC_RESETALL AC_REVERSED AC_RED_BLACK "%s" AC_RESETALL
 *            "|"
 *            "\n",
 *            i,
 *            ((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->size,
 *            __basename(((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename)
 *            );
 *    fprintf(stderr, "%s" AC_RESETALL, msg);
 *    free(msg);
 *  }
 *  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
 * }
 * djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
 * return(EXIT_SUCCESS);
 * } */

int parse_args(int argc, char *argv[]){
  char               identifier;
  const char         *value;
  cag_option_context context;
  size_t             palettes_age = timestamp() - EMBEDDED_PALETTES_TABLE_TIMESTAMP;

  if (DEBUG_PARSED_ARGS)
    printf(
      AC_RESETALL AC_REVERSED AC_YELLOW_BLACK AC_BOLD "%lu" AC_RESETALL " "
      AC_RESETALL "embedded palettes of" AC_RESETALL " "
      AC_RESETALL AC_REVERSED AC_GREEN "%s" AC_RESETALL " "
      "created"
      " "
      AC_RESETALL AC_REVERSED AC_MAGENTA "%s" AC_RESETALL
      " "
      "ago"
      "\n" AC_RESETALL,
      EMBEDDED_PALETTES_TABLE_QTY,
      bytes_to_string(EMBEDDED_PALETTES_TABLE_SIZE),
      milliseconds_to_string(palettes_age)
      );

  assert_ge(EMBEDDED_PALETTES_TABLE_QTY, 10, %u);
  assert_ge(EMBEDDED_PALETTES_TABLE_SIZE, 1024, %u);
  assert_le(palettes_age, 60 * 1000 * MAX_VALID_EMBEDDED_PALETTES_DATA_AGE_MINUTES, %u);

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);
    switch (identifier) {
    case 'm':
      value     = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'f':
      value     = cag_option_get_value(&context);
      args.file = value;
      break;
    case 'p':
      value        = cag_option_get_value(&context);
      args.palette = value;
      break;
    case 'P':
      args.pretty = true;
      break;
    case 'x':
      args.color = true;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'h':
      printf(AC_RESETALL AC_YELLOW AC_BOLD "Usage: cargsdemo [OPTION]...\n" AC_RESETALL);
      printf("Demonstrates the cargs library.\n\n");
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      printf("\nNote that all formatting is done by cargs.\n");
      return(EXIT_SUCCESS);
    }
  }

  return(EXIT_SUCCESS);
} /* parse_args */
