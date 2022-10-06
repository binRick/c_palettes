#pragma once
#include "sqlite3.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define PALETTEDB_INIT    NULL

typedef int64_t palettedb_id;

typedef struct {
  sqlite3      *db;
  sqlite3_stmt *select, *insert, *delete, *one;
  sqlite3_stmt *ids, *typeids, *typeid_ids, *count_typeids, *count_ids, *count_typeid;
  sqlite3_stmt *distinct_typeids;
} *palettedb;

typedef palettedb_id palettedb_type;

palettedb_id palettedb_add(palettedb db, const palettedb_type type, void *blob, size_t size);
void *palettedb_get(palettedb db, palettedb_id id, size_t *size);
void *palettedb_one(palettedb db, const palettedb_type type, palettedb_id *id, size_t *size);
void palettedb_delete(palettedb db, palettedb_id id);
palettedb palettedb_open(const char *path);
void palettedb_close(palettedb db);

void *palettedb_count_typeid(palettedb db, const palettedb_type type, size_t *size);
void *palettedb_count_typeids(palettedb db, size_t *size);
void *palettedb_count_ids(palettedb db, size_t *size);
void *palettedb_get_typeid_ids(palettedb db, const palettedb_type type, size_t *size, size_t *rows_qty);
void *palettedb_get_distinct_typeids(palettedb db, size_t *size, size_t *rows_qty);
