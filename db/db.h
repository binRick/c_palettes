#pragma once
#ifndef PALETTES_DB_H
#define PALETTES_DB_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
//////////////////////////////////////////////////////
#include "submodules/bytes/bytes.h"
#include "submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/c_stringfn/include/stringfn.h"
#include "submodules/fs.c/fs.h"
#include "submodules/parson/parson.h"
#include "vendor/sqlite3.h"
#include "db-mgr.h"
//////////////////////////////////////////////////////
#define PALETTEDB_MAX_HASH_BUCKETS 65535 * 256
//////////////////////////////////////////////////////
typedef struct PalettesDB PalettesDB;
struct PalettesDB {
  char    *Path;
  palettedb db;
};
//////////////////////////////////////////////////////
int palettesdb_list_ids(PalettesDB *DB);
int palettsdb_list_type_ids(PalettesDB *DB, palettedb_type TYPEID);
int palettesdb_count_ids(PalettesDB *DB);
int palettesdb_count_type_ids(PalettesDB *DB, palettedb_type TYPEID);
palettedb_id add_palettedb_if_not_exist(PalettesDB *DB, palettedb_type TYPEID, char *RECORD);
palettedb_id add_palettedb_(PalettesDB *DB, palettedb_type TYPEID, char *RECORD);
int init_palettes_db(PalettesDB *);
bool db_typeid_exists(PalettesDB *, palettedb_type TYPEID);
unsigned long palettedb_hash(char *key, int length);
int db_list_ids(PalettesDB *DB);
//////////////////////////////////////////////////////
#endif
