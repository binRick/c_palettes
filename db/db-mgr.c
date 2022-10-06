#include "db-mgr.h"
#include <stdio.h>

#define SELECT_IDS_QUERY                 "SELECT id FROM blobs"
#define SELECT_TYPEIDS_QUERY             "SELECT type FROM blobs"
#define SELECT_TYPEID_IDS_QUERY          "SELECT id from blobs where type = ?"

#define SELECT_COUNT_TYPEIDS_QUERY       "SELECT COUNT(DISTINCT type) from blobs"
#define SELECT_COUNT_IDS_QUERY           "SELECT COUNT(id) FROM blobs"
#define SELECT_COUNT_TYPEID_QUERY        "SELECT COUNT(id) from blobs where type = ?"
#define SELECT_DISTINCT_TYPEIDS_QUERY    "SELECT DISTINCT type from blobs"

#define SELECT_QUERY                     "SELECT data FROM blobs WHERE id = ?"
#define ONE_QUERY                        "SELECT id, data FROM blobs WHERE type = ? LIMIT 1"
#define INSERT_QUERY                     "INSERT INTO blobs(type, data) VALUES (?, ?)"
#define DELETE_QUERY                     "DELETE FROM blobs WHERE id = ?"
#define CREATE_TABLE_QUERY               "CREATE TABLE IF NOT EXISTS blobs (id INTEGER PRIMARY KEY, type INTEGER, data BLOB)"

void *palettedb_get_distinct_typeids(palettedb db, size_t *size, size_t *rows_qty){
  const void    *p;
  unsigned char *copy = NULL;

  *size     = 0;
  *rows_qty = 0;
  while (sqlite3_step(db->distinct_typeids) == SQLITE_ROW) {
    p = sqlite3_column_blob(db->distinct_typeids, 0);
    size_t        row_size  = (size_t)sqlite3_column_bytes(db->distinct_typeids, 0);
    unsigned char *row_data = malloc(row_size + 1);
    memcpy(row_data, p, row_size);
    row_data[row_size] = '\0';
    if (copy == NULL) {
      copy = malloc(row_size + 1);
    }else{
      copy = realloc(copy, (*size) + row_size + 1 + (*rows_qty));
    }
    if (!copy) {
      goto reset;
    }
    memcpy(copy + *size + (*rows_qty), p, row_size);
    copy[*size + (*rows_qty) + row_size] = '\0';
    *size                               += row_size;
    (*rows_qty)++;
  }
reset:
  sqlite3_clear_bindings(db->distinct_typeids);
  sqlite3_reset(db->distinct_typeids);
  return(copy);
}

void *palettedb_get_typeid_ids(palettedb db, const palettedb_type type, size_t *size, size_t *rows_qty){
  const void    *p;
  unsigned char *copy = NULL;

  *size     = 0;
  *rows_qty = 0;
  if (sqlite3_bind_int(db->typeid_ids, 1, type) != SQLITE_OK) {
    return(copy);
  }
  while (sqlite3_step(db->typeid_ids) == SQLITE_ROW) {
    p = sqlite3_column_blob(db->typeid_ids, 0);
    size_t        row_size  = (size_t)sqlite3_column_bytes(db->typeid_ids, 0);
    unsigned char *row_data = malloc(row_size + 1);
    memcpy(row_data, p, row_size);
    row_data[row_size] = '\0';
    if (copy == NULL) {
      copy = malloc(row_size + 1);
    }else{
      copy = realloc(copy, (*size) + row_size + 1 + (*rows_qty));
    }
    if (!copy) {
      goto reset;
    }
    memcpy(copy + *size + (*rows_qty), p, row_size);
    copy[*size + (*rows_qty) + row_size] = '\0';
    *size                               += row_size;
    (*rows_qty)++;
  }
reset:
  sqlite3_clear_bindings(db->typeid_ids);
  sqlite3_reset(db->typeid_ids);
  return(copy);
}

void *palettedb_count_ids(palettedb db, size_t *size){
  unsigned char *copy = NULL;

  if (sqlite3_step(db->count_ids) != SQLITE_ROW) {
    goto reset;
  }

  *size = sqlite3_column_int64(db->count_ids, 0);

reset:
  sqlite3_clear_bindings(db->count_ids);
  sqlite3_reset(db->count_ids);

  return(copy);
}

void *palettedb_count_typeids(palettedb db, size_t *size){
  unsigned char *copy = NULL;

  if (sqlite3_step(db->count_typeids) != SQLITE_ROW) {
    goto reset;
  }
  *size = sqlite3_column_int64(db->count_typeids, 0);
reset:
  sqlite3_clear_bindings(db->count_typeids);
  sqlite3_reset(db->count_typeids);
  return(copy);
}

void *palettedb_count_typeid(palettedb db, const palettedb_type type, size_t *size){
  unsigned char *copy = NULL;

  if (sqlite3_bind_int(db->count_typeid, 1, type) != SQLITE_OK) {
    return(copy);
  }

  if (sqlite3_step(db->count_typeid) != SQLITE_ROW) {
    goto reset;
  }

  *size = sqlite3_column_int64(db->count_typeid, 0);

reset:
  sqlite3_clear_bindings(db->count_typeid);
  sqlite3_reset(db->count_typeid);

  return(copy);
}

palettedb_id palettedb_add(palettedb            db,
                           const palettedb_type type,
                           void                 *blob,
                           size_t               size){
  sqlite3_int64 id = -1;

  if (size > INT_MAX) {
    return(id);
  }

  if (sqlite3_bind_int64(db->insert, 1, type) != SQLITE_OK) {
    return(id);
  }

  if (sqlite3_bind_blob(db->insert,
                        2,
                        blob,
                        (int)size,
                        SQLITE_STATIC) != SQLITE_OK) {
    goto reset;
  }

  if (sqlite3_step(db->insert) != SQLITE_DONE) {
    goto reset;
  }

  id = sqlite3_last_insert_rowid(db->db);

reset:
  sqlite3_clear_bindings(db->insert);
  sqlite3_reset(db->insert);

  return(id);
}

void *palettedb_get(palettedb db, palettedb_id id, size_t *size){
  const void    *p;
  unsigned char *copy = NULL;

  if (sqlite3_bind_int64(db->select, 1, id) != SQLITE_OK) {
    return(copy);
  }

  if (sqlite3_step(db->select) != SQLITE_ROW) {
    goto reset;
  }

  p     = sqlite3_column_blob(db->select, 0);
  *size = (size_t)sqlite3_column_bytes(db->select, 0);

  copy = malloc(*size + 1);
  if (!copy) {
    goto reset;
  }

  memcpy(copy, p, *size);
  copy[*size] = '\0';

reset:
  sqlite3_clear_bindings(db->select);
  sqlite3_reset(db->select);

  return(copy);
}

void *palettedb_one(palettedb            db,
                    const palettedb_type type,
                    palettedb_id         *id,
                    size_t               *size){
  const void    *p;
  unsigned char *copy = NULL;

  if (sqlite3_bind_int(db->one, 1, type) != SQLITE_OK) {
    return(copy);
  }

  if (sqlite3_step(db->one) != SQLITE_ROW) {
    goto reset;
  }

  *id = sqlite3_column_int64(db->one, 0);

  p     = sqlite3_column_blob(db->one, 1);
  *size = (size_t)sqlite3_column_bytes(db->one, 1);

  copy = malloc(*size + 1);
  if (!copy) {
    goto reset;
  }

  memcpy(copy, p, *size);
  copy[*size] = '\0';

reset:
  sqlite3_clear_bindings(db->one);
  sqlite3_reset(db->one);

  return(copy);
}

void palettedb_delete(palettedb db, palettedb_id id){
  if (sqlite3_bind_int64(db->delete, 1, id) != SQLITE_OK) {
    return;
  }

  sqlite3_step(db->delete);

  sqlite3_clear_bindings(db->delete);
  sqlite3_reset(db->delete);
}

static int try_again(__attribute__((unused)) void *arg, __attribute__((unused)) int times){
  return(1);
}

palettedb palettedb_open(const char *path){
  palettedb db;

  db = malloc(sizeof(*db));
  if (!db) {
    return(NULL);
  }

  if (sqlite3_open_v2(path,
                      &db->db,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                      NULL) != SQLITE_OK) {
    free(db);
    return(NULL);
  }

  if (sqlite3_busy_handler(db->db, try_again, NULL) != SQLITE_OK) {
    sqlite3_close(db->db);
    free(db);
  }

  if (sqlite3_exec(db->db,
                   CREATE_TABLE_QUERY,
                   NULL,
                   NULL,
                   NULL) != SQLITE_OK) {
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      INSERT_QUERY,
                      -1,
                      &db->insert,
                      NULL) != SQLITE_OK) {
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      SELECT_QUERY,
                      -1,
                      &db->select,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      ONE_QUERY,
                      -1,
                      &db->one,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      DELETE_QUERY,
                      -1,
                      &db->delete,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      SELECT_COUNT_TYPEID_QUERY,
                      -1,
                      &db->count_typeid,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->delete);
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      SELECT_COUNT_TYPEIDS_QUERY,
                      -1,
                      &db->count_typeids,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->count_typeid);
    sqlite3_finalize(db->delete);
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }
  if (sqlite3_prepare(db->db,
                      SELECT_COUNT_IDS_QUERY,
                      -1,
                      &db->count_ids,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->count_typeids);
    sqlite3_finalize(db->count_typeid);
    sqlite3_finalize(db->delete);
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }
  if (sqlite3_prepare(db->db,
                      SELECT_TYPEID_IDS_QUERY,
                      -1,
                      &db->typeid_ids,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->count_ids);
    sqlite3_finalize(db->count_typeids);
    sqlite3_finalize(db->count_typeid);
    sqlite3_finalize(db->delete);
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }
  if (sqlite3_prepare(db->db,
                      SELECT_DISTINCT_TYPEIDS_QUERY,
                      -1,
                      &db->distinct_typeids,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->typeid_ids);
    sqlite3_finalize(db->count_ids);
    sqlite3_finalize(db->count_typeids);
    sqlite3_finalize(db->count_typeid);
    sqlite3_finalize(db->delete);
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  return(db);
} /* palettedb_open */

void palettedb_close(palettedb db){
  if (!db) {
    return;
  }

  sqlite3_finalize(db->distinct_typeids);
  sqlite3_finalize(db->typeid_ids);
  sqlite3_finalize(db->count_ids);
  sqlite3_finalize(db->count_typeids);
  sqlite3_finalize(db->count_typeid);
  sqlite3_finalize(db->delete);
  sqlite3_finalize(db->one);
  sqlite3_finalize(db->select);
  sqlite3_finalize(db->insert);

  sqlite3_close(db->db);
  free(db);
}
