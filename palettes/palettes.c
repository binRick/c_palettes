#include "palettes.h"
#define DEBUG_PALETTE_NAMES    false
//////////////////////////////////////////////
static const char *PALETTE_TYPE_NAMES[] = {
  "paleta",
  "kfc",
  NULL,
};


char *get_palette_property_value(const char *PROPERTY_NAME, const char *PALETTE_DATA){
  char *PROPERTY_VALUE = malloc(HEX_LEN);

  sprintf(PROPERTY_VALUE, "NONE");
  struct StringFNStrings PALETTE_LINES = stringfn_split_lines_and_trim(PALETTE_DATA);

  switch (get_palette_data_type(PALETTE_DATA)) {
  case PALETTE_TYPE_PALETA:
    if (strcmp(PROPERTY_NAME, "background")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[0]));
    }
    if (strcmp(PROPERTY_NAME, "foreground")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[1]));
    }
    if (strcmp(PROPERTY_NAME, "cursor")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[2]));
    }
    if (strcmp(PROPERTY_NAME, "color00")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[3]));
    }
    if (strcmp(PROPERTY_NAME, "color01")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[4]));
    }
    if (strcmp(PROPERTY_NAME, "color02")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[5]));
    }
    if (strcmp(PROPERTY_NAME, "color03")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[6]));
    }
    if (strcmp(PROPERTY_NAME, "color04")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[7]));
    }
    if (strcmp(PROPERTY_NAME, "color05")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[8]));
    }
    if (strcmp(PROPERTY_NAME, "color06")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[9]));
    }
    if (strcmp(PROPERTY_NAME, "color07")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[10]));
    }
    if (strcmp(PROPERTY_NAME, "color08")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[11]));
    }
    if (strcmp(PROPERTY_NAME, "color09")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[12]));
    }
    if (strcmp(PROPERTY_NAME, "color10")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[13]));
    }
    if (strcmp(PROPERTY_NAME, "color11")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[14]));
    }
    if (strcmp(PROPERTY_NAME, "color12")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[15]));
    }
    if (strcmp(PROPERTY_NAME, "color13")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[16]));
    }
    if (strcmp(PROPERTY_NAME, "color14")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[17]));
    }
    if (strcmp(PROPERTY_NAME, "color15")) {
      sprintf(PROPERTY_VALUE, "%s", strdup(PALETTE_LINES.strings[18]));
    }
    break;
  case PALETTE_TYPE_KFC:
    for (int i = 0; i < PALETTE_LINES.count - 1; i++) {
      struct StringFNStrings LINE_SPLIT = stringfn_split(PALETTE_LINES.strings[i], '=');
      if (LINE_SPLIT.count == 2 && (strcmp(LINE_SPLIT.strings[0], PROPERTY_NAME) == 0)) {
        sprintf(PROPERTY_VALUE, "%s", strdup(LINE_SPLIT.strings[1]));
      }
      stringfn_release_strings_struct(LINE_SPLIT);
    }
    break;
  } /* switch */
  stringfn_release_strings_struct(PALETTE_LINES);
  return(stringfn_to_uppercase(PROPERTY_VALUE));
} /* get_palette_property_value */


struct Palette get_palette(char *PALETTE_DATA){
  struct Palette P = {
    .Name     = "UNKNOWN",
    .Type     = get_palette_data_type(PALETTE_DATA),
    .TypeName = get_palette_data_type_name(PALETTE_DATA),
    .fgColors = malloc(sizeof(AnsiColors)),
    .bgColors = malloc(sizeof(AnsiColors)),
  };

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  sprintf(&P.fg, "%s", (char *)get_palette_property_value("foreground", PALETTE_DATA));
  sprintf(&P.bg, "%s", (char *)get_palette_property_value("background", PALETTE_DATA));
  sprintf(&P.cursor, "%s", (char *)get_palette_property_value("cursor", PALETTE_DATA));
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  sprintf(&P.fgColors->black, "%s", (char *)get_palette_property_value("color00", PALETTE_DATA));
  sprintf(&P.fgColors->red, "%s", (char *)get_palette_property_value("color01", PALETTE_DATA));
  sprintf(&P.fgColors->green, "%s", (char *)get_palette_property_value("color02", PALETTE_DATA));
  sprintf(&P.fgColors->yellow, "%s", (char *)get_palette_property_value("color03", PALETTE_DATA));
  sprintf(&P.fgColors->blue, "%s", (char *)get_palette_property_value("color04", PALETTE_DATA));
  sprintf(&P.fgColors->magenta, "%s", (char *)get_palette_property_value("color05", PALETTE_DATA));
  sprintf(&P.fgColors->cyan, "%s", (char *)get_palette_property_value("color06", PALETTE_DATA));
  sprintf(&P.fgColors->white, "%s", (char *)get_palette_property_value("color07", PALETTE_DATA));
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  sprintf(&P.bgColors->black, "%s", (char *)get_palette_property_value("color08", PALETTE_DATA));
  sprintf(&P.bgColors->red, "%s", (char *)get_palette_property_value("color09", PALETTE_DATA));
  sprintf(&P.bgColors->green, "%s", (char *)get_palette_property_value("color10", PALETTE_DATA));
  sprintf(&P.bgColors->yellow, "%s", (char *)get_palette_property_value("color11", PALETTE_DATA));
  sprintf(&P.bgColors->blue, "%s", (char *)get_palette_property_value("color12", PALETTE_DATA));
  sprintf(&P.bgColors->magenta, "%s", (char *)get_palette_property_value("color13", PALETTE_DATA));
  sprintf(&P.bgColors->cyan, "%s", (char *)get_palette_property_value("color14", PALETTE_DATA));
  sprintf(&P.bgColors->white, "%s", (char *)get_palette_property_value("color15", PALETTE_DATA));
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  switch (get_palette_data_type(PALETTE_DATA)) {
  case PALETTE_TYPE_PALETA:
    P.Parsed = true;
    sprintf(&P.fgColors->black, "kk");
    break;
  case PALETTE_TYPE_KFC:
    P.Parsed = true;
    break;
  }
  return(P);
} /* get_palette */


char *get_palette_data_type_name(char *PALETTE_DATA){
  return(PALETTE_TYPE_NAMES[get_palette_data_type(PALETTE_DATA)]);
}


int get_palette_data_type(char *PALETTE_DATA){
  struct StringFNStrings PALETTE_LINES = stringfn_split_lines_and_trim(PALETTE_DATA);

  for (int i = 0; i < PALETTE_LINES.count - 1; i++) {
    struct StringFNStrings LINE_SPLIT = stringfn_split(PALETTE_LINES.strings[i], '=');
    if (DEBUG_PALETTE_NAMES) {
      fprintf(stderr,
              AC_RESETALL AC_YELLOW AC_REVERSED "%s\n" AC_RESETALL,
              PALETTE_LINES.strings[i]
              );
      fprintf(stderr, "%d/%d\n", LINE_SPLIT.count, PALETTE_LINES.count);
    }
    if (LINE_SPLIT.count == 1) {
      return(PALETTE_TYPE_PALETA);
    }
    if (LINE_SPLIT.count == 2) {
      return(PALETTE_TYPE_KFC);
    }
  }
  return(-1);
}

/*
 * Palette KFCParser(char *PALETTE_FILE_PATH){
 *  Palette *P = malloc(sizeof(Palette));
 *  return *P;
 *
 * }
 *
 * Palette ParsePalette(PaletteParser_t Parser, char *PALETTE_FILE_PATH){
 *
 *  Palette *P = malloc(sizeof(Palette));
 *  return *P;
 * }
 */

/*
 * int main(int argc, char **argv) {
 * if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
 *  printf(AC_RESETALL AC_GREEN "OK\n" AC_RESETALL);
 *  printf(acsl(AC_RESETALL AC_GREEN "OK"));
 *  return(0);
 * }
 *
 * if ((strcmp(argv[1], "--load") == 0)) {
 *  //return(lph());
 *  return(load_palettes_hash());
 * }
 * return(1);
 * }
 */
