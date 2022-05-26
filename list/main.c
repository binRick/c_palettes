#include "palettes.h"
/////////////////////////////////////////////////////////////////////////////////////////////
#define MIN_PALETTE_SIZE 32
/////////////////////////////////////////////////////////////////////////////////////////////
#define DEBUG_PALETTE_CONTENT_COLORS             false
#define DEBUG_PALETTE_PRETTY_JSON_CONTENT        false
#define DEBUG_PALETTE_JSON_CONTENT               false
#define EMBEDDED_PALETTES_TABLE_DEBUG            false
#define EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG    false
/////////////////////////////////////////////////////////////////////////////////////////////
#define EMBEDDED_PALETTES_TABLE_ITEM             embedded_palettes_table_item
#define EMBEDDED_PALETTES_TABLE_HASH             embedded_palettes_table_hash
#define EMBEDDED_PALETTES_TABLE_TIMESTAMP        embedded_palettes_table_created_ts
#define EMBEDDED_PALETTES_TABLE_NAME             embedded_palettes_table
#define EMBEDDED_PALETTES_TABLE_SIZE             embedded_palettes_table_bytes
#define EMBEDDED_PALETTES_TABLE_QTY              embedded_palettes_table_qty-1
/////////////////////////////////////////////////////////////////////////////////////////////
struct djbhash_node *EMBEDDED_PALETTES_TABLE_ITEM;
struct djbhash      EMBEDDED_PALETTES_TABLE_HASH;
args_t              args = {
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
  DEFAULT_PRETTY,
  DEFAULT_COLOR,
};
/////////////////////////////////////////////////////////////////////////////////////////////


int parse_embedded_palettes(){
  if (EMBEDDED_PALETTES_TABLE_DEBUG) {
    printf("Parsing Palettes\n");
  }
  int         i = 0; EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  uint32_t    r;
  JSON_Value  *o;
  JSON_Object *O;
  short       ok;
  char        *LINE, *NAME, *HEX, *colp, *ptr, *js;
  int         qty = 0, ui, palettes_qty = 0;

  while (EMBEDDED_PALETTES_TABLE_ITEM && (++i < EMBEDDED_PALETTES_TABLE_QTY)) {
    char                   *palette      = __basename((char *)((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename);
    size_t                 palette_size  = (size_t)((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->size;
    char                   *palette_data = (char *)((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->data;
    if(strlen(palette_data)<MIN_PALETTE_SIZE){
continue;
    }
    struct StringFNStrings palette_lines = stringfn_split_lines_and_trim(palette_data);
    if (EMBEDDED_PALETTES_TABLE_VERBOSE_DEBUG) {
      fprintf(stdout,
              AC_RESETALL AC_REVERSED AC_MAGENTA_BLACK "%s" AC_RESETALL
              AC_RESETALL AC_REVERSED AC_RED_BLACK AC_BOLD "%d" AC_RESETALL
              AC_RESETALL " " AC_REVERSED AC_GREEN_BLACK AC_BOLD "Lines" AC_RESETALL
              "\n",
              palette_data,
              palette_lines.count
              );
    }
    char *palette_line;
    for (int li = 0; li < palette_lines.count; li++) {
      palette_line = stringfn_trim(palette_lines.strings[li]);
      if (strlen(palette_line) < 1) {
        continue;
      }
      struct StringFNStrings palette_line_split = stringfn_split(palette_line, '=');
      if (palette_line_split.count != 2) {
        continue;
      }
      char *palette_line_key = strdup(palette_line_split.strings[0]);
      char *palette_line_val = strdup(stringfn_to_uppercase(palette_line_split.strings[1]));
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
      if (EMBEDDED_PALETTES_TABLE_DEBUG) {
        fprintf(stdout, AC_UNDERLINE AC_YELLOW_BLACK "RGB" AC_RESETALL
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
      js = json_serialize_to_string(o);
      assert_nonnull(js);
      ////////////////////////////////////////////////////
      if (DEBUG_PALETTE_CONTENT_COLORS) {
        ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
      }
      if (DEBUG_PALETTE_PRETTY_JSON_CONTENT) {
        char *pjs = json_serialize_to_string_pretty(o);
        printf("%s\n", pjs);
        free(pjs);
      }else if (DEBUG_PALETTE_JSON_CONTENT) {
        fprintf(stdout, "%s\n", js);
      }
      if (DEBUG_PALETTE_CONTENT_COLORS) {
        ansi_reset(stdout);
      }
      free(js);
      ///////////////////////////////////////////////////////////////////////////////////////////
    }

    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
    palettes_qty++;
  }
  printf("Parsed %d Palette Lines from %d Palettes\n",
         qty, palettes_qty
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
          "\n",
          args.verbose, args.mode, args.count, args.pretty, args.color
          );

  return(EXIT_SUCCESS);
}


int main(int argc, char **argv) {
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }

  int r;

  r = parse_args(argc, argv);
  assert_eq(r, 0, %d);
  r = debug_args();
  assert_eq(r, 0, %d);


  ct_set_unit(ct_SECONDS);
  ct_start(NULL);
  r = load_palettes_hash();
  assert_eq(r, 0, %d);

  char *dur0 = ct_stop("load palettes to hash");

  ct_start(NULL);
  r = parse_embedded_palettes();
  assert_eq(r, 0, %d);
  char *dur1 = ct_stop("parse_embedded_palettes");

  dbg(dur0, %s);
  dbg(dur1, %s);
  exit(0);

  if ((strcmp(args.mode, "parse") == 0)) {
    return(parse_embedded_palettes());
  }
  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode); return(1);

  return(EXIT_SUCCESS);
}


int load_palettes_hash(){
  djbhash_init(&EMBEDDED_PALETTES_TABLE_HASH);
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  for (int i = 0; i < EMBEDDED_PALETTES_TABLE_QTY; i++) {
    if (strlen(EMBEDDED_PALETTES_TABLE_NAME[i].filename) < 1) {
      continue;
    }
    djbhash_set(&EMBEDDED_PALETTES_TABLE_HASH, EMBEDDED_PALETTES_TABLE_NAME[i].filename, &EMBEDDED_PALETTES_TABLE_NAME[i], DJBHASH_OTHER);
  }
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);


  for (int i = 0; EMBEDDED_PALETTES_TABLE_ITEM; i++) {
    if (EMBEDDED_PALETTES_TABLE_DEBUG) {
      char *msg = malloc(4096);
      sprintf(msg,
              AC_YELLOW_BLACK AC_REVERSED "#%d" AC_RESETALL
              "> "
              "|"
              "size:"
              AC_BLUE_BLACK AC_REVERSED "%lu" AC_RESETALL
              "|"
              "filename:"
              AC_RESETALL AC_REVERSED AC_RED_BLACK "%s" AC_RESETALL
              "|"
              "\n",
              i,
              ((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->size,
              __basename(((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename)
              );
      fprintf(stderr, "%s" AC_RESETALL, msg);
      free(msg);
    }
    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  }
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
  return(EXIT_SUCCESS);
} /* load_palettes_hash */


int parse_args(int argc, char *argv[]){
  char               identifier;
  const char         *value;
  cag_option_context context;
  size_t             palettes_age = timestamp() - EMBEDDED_PALETTES_TABLE_TIMESTAMP;

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

  assert_ge(EMBEDDED_PALETTES_TABLE_QTY, 10, %lu);
  assert_ge(EMBEDDED_PALETTES_TABLE_SIZE, 1024, %lu);
  assert_le(palettes_age, 60 * 1000, %lu);

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);
    switch (identifier) {
    case 'm':
      value     = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'p':
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

