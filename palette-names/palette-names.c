#include "embeds/tbl1.c"
#include "palette-names.h"
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

struct StringFNStrings pn_get_palette_files(){
  struct StringBuffer *sb = stringbuffer_new_with_options(1024, true);

  EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  for (int i = 0; EMBEDDED_PALETTES_TABLE_ITEM; i++) {
    if (strlen(EMBEDDED_PALETTES_TABLE_NAME[i].filename) < 1)
      continue;
    stringbuffer_append_string(sb,
                               ((C_EMBED_TBL *)(EMBEDDED_PALETTES_TABLE_ITEM)->value)->filename
                               );
    stringbuffer_append_string(sb, "\n");
    fprintf(stdout, "%s\n", stringbuffer_to_string(sb));
    EMBEDDED_PALETTES_TABLE_ITEM = djbhash_iterate(&EMBEDDED_PALETTES_TABLE_HASH);
  }
  djbhash_reset_iterator(&EMBEDDED_PALETTES_TABLE_HASH);
//  return(stringbuffer_to_strings(sb));
}

void pn_get_palette_names(){
  struct StringBuffer    *sb = stringbuffer_new_with_options(1024, true);
  struct StringFNStrings S   = pn_get_palette_files();

  for (int i = 0; i < S.count; i++) {
    if (strlen(stringfn_trim(S.strings[i])) < 1)
      continue;
    stringbuffer_append_string(sb, stringfn_trim(S.strings[i]));
    stringbuffer_append_string(sb, "\n");
  }
  //return(stringbuffer_to_strings(sb));
}

int main(int argc, char **argv) {
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf(AC_RESETALL AC_GREEN "OK\n" AC_RESETALL);
    printf(acsl(AC_RESETALL AC_GREEN "OK"));
    return(0);
  }

  if ((argc >= 2) && (strcmp(argv[1], "--load") == 0)) {
    //return(lph());
    //   return(load_palettes_hash());
  }
  pn_get_palette_files();
  printf("kk\n");
  return(0);
}
