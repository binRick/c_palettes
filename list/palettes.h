#pragma once
#ifndef PALETTES_LIST_H
#define PALETTES_LIST_H
#include "../embeds/tbl1.c"
#include "list.h"
////////////////////////////////////////////
int load_palettes_hash();
int parse_embedded_palettes();
int parse_args(int, char **);
int debug_args();

////////////////////////////////////////////
#include "palettes.c"
#include "utils.c"
#endif
