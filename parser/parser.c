#include "parser.h"
args_t args = {
  DEFAULT_INPUT,
  DEFAULT_OUTPUT_DIR,
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
};
PalettesDB *DB;

int main(int argc, char **argv) {
  parse_args(argc, argv);
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf(AC_RESETALL AC_GREEN "OK\n" AC_RESETALL);
    return(EXIT_SUCCESS);
  }else if ((strcmp(args.mode, "args") == 0)) {
      return(debug_args());
  }

  int loaded = load_palettes_hash();
  assert_eq(loaded, 0, %d);

  if ((strcmp(args.mode, "load") == 0)) {
      return(loaded);
  }else if ((strcmp(args.mode, "parse") == 0)) {
      return(parse_embedded_palettes());
  }

  DB                 = malloc(sizeof(PalettesDB));
  DB->Path           = PALETTES_DB_PATH;
  loaded = init_palettes_db(DB);
  assert_eq(loaded, 0, %d);
  struct StringFNStrings Files = get_palette_files();
  struct StringFNStrings Names = get_palette_names();


  if ((strcmp(args.mode, "files") == 0)) {
      printf("Loaded %d Files\n", Files.count);
      for(int i=0;i<args.count;i++)
        printf(" - #%d %s\n", i, Files.strings[i]);
      return((Files.count > 0) ? 0 : 1);
  }else if ((strcmp(args.mode, "names") == 0)) {
      printf("Loaded %d Names\n", Names.count);
      for(int i=0;i<args.count;i++)
        printf(" - #%d %s\n", i, Names.strings[i]);
      return((Names.count > 0) ? 0 : 1);
  }else if ((strcmp(args.mode, "hash") == 0)) {
      for(int i=0;i<args.count;i++){
        unsigned long palette_hash = palettedb_hash(Names.strings[i], strlen(Names.strings[i]));
        printf(" - #%d %s -> %lu\n"
                , i
                , Names.strings[i]
                , palette_hash
                );
      }
      return(0);
  }else if ((strcmp(args.mode, "ids") == 0)) {
      size_t db_ids_qty = 0;
      palettedb_count_ids(DB->db, &db_ids_qty);
      printf("Palette DB Has %lu IDs\n", db_ids_qty);
      return(0);
  }else if ((strcmp(args.mode, "sync") == 0)) {
      printf("Syncing %d Palettes\n", args.count);
      for(int i=0;i<args.count && i < Files.count;i++){
        char *palette_content;
        bool exists = palette_file_exists(Files.strings[i]);
        struct StringFNStrings PaletteLines = palette_file_content(Files.strings[i]);
        struct StringBuffer *PaletteContentBuffer = strings_to_stringbuffer(PaletteLines);
        unsigned long palette_hash = palettedb_hash(
                stringbuffer_to_string(PaletteContentBuffer),
                stringbuffer_get_content_size(PaletteContentBuffer)
                );
        bool row_exists = db_typeid_exists(DB->db, (palettedb_type)palette_hash);
        if(!row_exists){
            palette_content = stringbuffer_to_string(PaletteContentBuffer);
            palettedb_id id = add_palettedb_if_not_exist(DB->db, palette_hash, (void *)palette_content);
            row_exists = db_typeid_exists(DB->db, (palettedb_type)palette_hash);
            free(palette_content);
        }
        printf(" - #%d %25s -> %lu :: Exists ? %s | "
                "Content Lines: %d | Content Size: %lubytes | "
                "Row exists? %s | "
                AC_RESETALL "\n"
                , i
                , Names.strings[i]
                , palette_hash
                , exists ? "Yes" : "No"
                , PaletteLines.count
                , stringbuffer_get_content_size(PaletteContentBuffer)
                , row_exists ? "Yes" : "No"
                );
        stringbuffer_release(PaletteContentBuffer);
        stringfn_release_strings_struct(PaletteLines);
      }
      return(0);
  }else if ((strcmp(args.mode, "db") == 0)) {
      printf("Querying DB\n", args.count);
      return(db_list_ids(DB));
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);
}


int debug_args(){
    fprintf(stderr,
    acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d")
    ", "
    ansistr(AC_RESETALL AC_REVERSE AC_BRIGHT_RED_BLACK "Input: %s")
    ", "
    ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Output Directory: %s")
    ", "
    ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Mode: %s")
    ", "
    ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Count: %d")
    ", "
    "\n"
    , args.verbose, args.input, args.output_dir, args.mode, args.count
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
      value      = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'i':
      value      = cag_option_get_value(&context);
      args.input = value;
      break;
    case 'o':
      value       = cag_option_get_value(&context);
      args.output_dir = value;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: hex-png-pixel-creator [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  return(EXIT_SUCCESS);
} /* parse_args */
