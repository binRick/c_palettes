#define INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_PREFIX    __THEME__
//////////////////
#include "palettes-test.h"
//////////////////
struct djbhash PalettesHash;
extern char    *EMBEDDED_PALETTE_NAMES[];
extern int     EMBEDDED_PALETTE_NAMES_QTY;
//////////////////
#define BIN_DATA(NAME) \
  __THEME__ ## NAME ## _json__ ## Data
#define LOADBIN(NAME)             \
  INCBIN_EXTERN(NAME ## _json__); \
//////////////////////////////
#define GET_NAME_JSON_OBJECT(NAME) \
  (JSON_Object *)(djbhash_find(&PalettesHash, #NAME)->value)
//////////////////////////////
#define GET_NAME_JSON_OBJECT_PROPS_QTY(NAME) \
  json_object_get_count(GET_NAME_JSON_OBJECT(NAME))
//////////////////////////////
#define GET_NAME_JSON_OBJECT_PROP(NAME, PROP) \
  json_object_get_string(GET_NAME_JSON_OBJECT(NAME), PROP)
//////////////////////////////
#define SET_NAME_HASH_ITEM(NAME)                                                                              \
  djbhash_set(&PalettesHash, #NAME, json_value_get_object(json_parse_string(BIN_DATA(NAME))), DJBHASH_OTHER); \
//////////////////////////////
#define LOADJSON(NAME)    { do{                                                                                                 \
                              SET_NAME_HASH_ITEM(NAME);                                                                         \
                              fprintf(stderr, "JSON Parsed Props        :%lu\n", GET_NAME_JSON_OBJECT_PROPS_QTY(NAME));         \
                              fprintf(stderr, "JSON Parsed Name         :%s\n", GET_NAME_JSON_OBJECT_PROP(NAME, "name"));       \
                              fprintf(stderr, "JSON Parsed Name         :%s\n", GET_NAME_JSON_OBJECT_PROP(NAME, "background")); \
                              fprintf(stderr, "JSON Parsed Green        :%s\n", GET_NAME_JSON_OBJECT_PROP(NAME, "green"));      \
                            }while (0); }
//////////////////////////////
#define DEBUG_INCBIN(NAME)                                            \
  fprintf(stderr, "Name:%s\n",             # NAME);                   \
  fprintf(stderr, "Size:%d\n", __THEME__ ## NAME ## _json__ ## Size); \
  fprintf(stderr, "DATA:%s\n", (char *)BIN_DATA(NAME));               \
  LOADJSON(NAME);                                                     \
//////////////////////////////////////////////
#define LOADBINS()    \
  LOADBIN(matrix);    \
  LOADBIN(Vaughn);    \
  LOADBIN(ayu_light); \
//////////////////
#define DEBUG_INCBINS()    { do{                                                            \
                               djbhash_init(&PalettesHash);                                 \
                               DEBUG_INCBIN(matrix);                                        \
                               DEBUG_INCBIN(Vaughn);                                        \
                               DEBUG_INCBIN(ayu_light);                                     \
                               fprintf(stderr, "Hash Qty:%d\n", PalettesHash.active_count); \
                               djbhash_destroy(&PalettesHash);                              \
                               for (int i = 0; i < EMBEDDED_PALETTE_NAMES_QTY; i++)         \
                               printf(">pn:%s\n", EMBEDDED_PALETTE_NAMES[i]);               \
                             }while (0); }
//////////////////
LOADBINS();

int list_palette_types();
int list_palettes();
int debug_args();
int parse_args(int argc, char **argv);

////////////////////////////////////////////////////////////////////
args_t args = {
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
  DEFAULT_PALETTE_TYPE,
  DEFAULT_PALETTE,
  DEFAULT_PALETTE_PROPERTY,
  DEFAULT_PRETTY_MODE,
};
////////////////////////////////////////////////////////////////////


int list_palettes(){
  Palette *P;

  if (args.verbose) {
    dbg(embedded_palettes_table_qty, %lu);
  }
  for (int i = 0; i < embedded_palettes_table_qty && i < args.count; i++) {
    Palette _p = get_palette(embedded_palettes_table[i].data); P = &_p;
    if (!P) {
      continue;
    }
    bool name_match = wildcardcmp(args.palette, palettes_basename(embedded_palettes_table[i].filename)),
         type_match = wildcardcmp(args.palette_type, P->TypeName)
    ;
    if (type_match != 1 || name_match != 1) {
      continue;
    }

    fprintf(stdout, "%s\n", palettes_basename(embedded_palettes_table[i].filename));
    if (args.pretty) {
      fprintf(stdout, "%s\n", P->JSON_PRETTY);
    }else{
      fprintf(stdout, "%s\n", P->JSON);
    }

    if (args.verbose) {
      dbg(name_match, %d);
      dbg(type_match, %d);

      dbg(embedded_palettes_table[i].filename, %s);
      dbg(palettes_basename(embedded_palettes_table[i].filename), %s);
      dbg(embedded_palettes_table[i].size, %lu);
      dbg(strlen(embedded_palettes_table[i].data), %lu);

      dbg((int)P->Type, %d);
      dbg((char *)P->Name, %s);
      dbg((bool)P->Parsed, %d);
      dbg((bool)P->Valid, %d);
      dbg((char *)P->TypeName, %s);

      dbg((char *)P->fgSelection, %s);
      dbg((char *)P->bgSelection, %s);

      dbg((char *)P->fgColors->black, %s);
      dbg((char *)P->fgColors->red, %s);
      dbg((char *)P->bgColors->black, %s);
      dbg((char *)P->bgColors->red, %s);

      dbg((char *)P->cursor, %s);

      dbg((char *)P->fg, %s);
      dbg((char *)P->bg, %s);
    }
  }
  return(EXIT_SUCCESS);
} /* list_palettes */


int list_palette_types(){
  if (args.verbose) {
    dbg(PALETTE_TYPE_NAMES_QTY, %d);
  }
  for (int i = 0; i < PALETTE_TYPE_NAMES_QTY && i < args.count; i++) {
    bool type_match = wildcardcmp(args.palette_type, PALETTE_TYPE_NAMES[i]);
    if (type_match != 1) {
      continue;
    }
    fprintf(stdout, "%s\n", PALETTE_TYPE_NAMES[i]);
    if (args.verbose) {
      dbg(type_match, %d);
      dbg(PALETTE_TYPE_NAMES[i], %s);
    }
  }
  return(EXIT_SUCCESS);
}


int debug_args(){
  fprintf(stdout,
          ansistr(AC_RESETALL AC_REVERSE AC_BRIGHT_RED_BLACK "Mode: %s") ", "
          acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d") ", "
          ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Count: %d") ", "
          ansistr(AC_RESETALL AC_BRIGHT_YELLOW_BLACK AC_ITALIC "Palette: %s") ", "
          ansistr(AC_RESETALL AC_BRIGHT_BLUE_BLACK AC_ITALIC "Palette Type: %s") ", "
          ansistr(AC_RESETALL AC_BRIGHT_RED_BLACK AC_ITALIC "Palette Property: %s") ", "
          acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Pretty Mode: %d") ", "
          "\n",
          args.mode,
          args.verbose,
          args.count,
          args.palette,
          args.palette_type,
          args.palette_property,
          args.pretty
          );

  return(EXIT_SUCCESS);
}


int parse_args(int argc, char **argv){
  char               identifier;
  const char         *value;
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);

    switch (identifier) {
    case 'm':
      value     = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 't':
      value             = cag_option_get_value(&context);
      args.palette_type = value;
      break;
    case 'p':
      value        = cag_option_get_value(&context);
      args.palette = value;
      break;
    case 'P':
      value                 = cag_option_get_value(&context);
      args.palette_property = value;
      break;
    case 'z':
      args.pretty = true;
      break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: hex-png-pixel-creator [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  return(EXIT_SUCCESS);
} /* parse_args */


int main(int argc, char **argv) {
  (void)argc; (void)argv;
  parse_args(argc, argv);
  DEBUG_INCBINS();
//  fprintf(stderr,"%s\n",gtheme_synthwaveData);
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }
  if ((strcmp(args.mode, "args") == 0)) {
    return(debug_args());
  }
  if ((strcmp(args.mode, "types") == 0)) {
    return(list_palette_types());
  }
  if ((strcmp(args.mode, "list") == 0)) {
    return(list_palettes());
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);
}

#undef INCBIN_PREFIX
