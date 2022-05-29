#include "db.h"

unsigned long palettedb_hash(char *key, int length){
  unsigned long i;
  unsigned long hash;

  hash = 5381;
  for ( i = 0; i < length; key++, i++ ) {
    hash = ((hash << 5) + hash) + (*key);
  }
  return(hash % PALETTEDB_MAX_HASH_BUCKETS);
}


int db_list_ids(PalettesDB *DB){
  size_t       total_ids = 0, unique_typeids_qty = 0, typeid_qty = 0, type_ids_size = 0, type_ids_qty = 0, unique_typeids_size;
  char *tmp;
  palettedb_type TYPEID = 2588928;

  unique_typeids_qty  = 0;
  unique_typeids_size = 0;
  char *unique_typeids = (char *)palettedb_get_distinct_typeids(DB->db, &unique_typeids_size, &unique_typeids_qty);

  printf("found %lu rows, %lu bytes of unique typeids\n",
         unique_typeids_qty,
         unique_typeids_size
         );

  tmp = unique_typeids;
  for (size_t processed_items = 0; processed_items < unique_typeids_qty; ) {
    if (strlen(tmp) > 0) {
      palettedb_type row_typeid    = atoi(tmp);
      size_t       row_data_size = 0;
      printf("  item #%lu> %s (%lu bytes)  row typeid: %llu- %lu bytes\n", processed_items, tmp, strlen(tmp), row_typeid, row_data_size);
      size_t       row_type_ids_size = 0, row_type_ids_qty = 0;
      char         *id_type_ids = (char *)palettedb_get_typeid_ids(DB->db, row_typeid, &row_type_ids_size, &row_type_ids_qty);
      printf("found %lu rows, %lu bytes of data for %llu ids list query\n",
             row_type_ids_qty,
             row_type_ids_size,
             row_typeid
             );
      char *_tmp = id_type_ids;
      for (size_t _processed_items = 0; _processed_items < row_type_ids_qty; ) {
        if (strlen(_tmp) > 0) {
          int    row_id        = atoi(_tmp);
          size_t row_data_size = 0;
          char   *row_data     = palettedb_get(DB->db, row_id, &row_data_size);
          printf(AC_RESETALL AC_RED "\t  item #%lu> %s (%lu bytes)  #%d- %lu bytes\n" AC_RESETALL,
                 _processed_items,
                 _tmp,
                 strlen(_tmp),
                 row_id,
                 row_data_size
                 );
          printf("\t  - " AC_RESETALL AC_YELLOW AC_ITALIC "%s" AC_RESETALL "\n", row_data);
          _tmp += strlen(_tmp);
        }else{
          _processed_items++;
          _tmp++;
        }
      }
      tmp += strlen(tmp);
    }else{
      processed_items++;
      tmp++;
    }
  }


  palettedb_count_typeid(DB->db, TYPEID, &typeid_qty);
  printf("type %llu has %lu items\n", TYPEID, typeid_qty);

  palettedb_count_typeids(DB->db, &unique_typeids_qty);
  printf("there are  %lu unique typeids\n", unique_typeids_qty);

  palettedb_count_ids(DB->db, &total_ids);
  printf("there are  %lu total ids\n", total_ids);
  return(0);
} /* db_list_ids */


palettedb_id db_get_typeid_id(PalettesDB *DB, palettedb_type TYPEID){
  palettedb_id id  = 0;
  size_t     len = 0;

  palettedb_one(DB, TYPEID, &id, &len);
  if ((len > 0) && (id > 0)) {
    return(id);
  }
  return(-1);
}


bool db_typeid_exists(PalettesDB *DB, palettedb_type TYPEID){
  return(db_get_typeid_id(DB, TYPEID) > 0);
}


palettedb_id add_palettes_db(PalettesDB *DB, palettedb_type TYPEID, char *RECORD){
  palettedb_id id = palettedb_add(DB, TYPEID, (void *)RECORD, strlen(RECORD));

  return(id);
}


palettedb_id add_palettedb_if_not_exist(PalettesDB *DB, palettedb_type TYPEID, char *RECORD){
  palettedb_id id = db_get_typeid_id(DB, TYPEID);

  if (id > 0) {
    return(id);
  }
  return(add_palettes_db(DB, TYPEID, RECORD));
}


int init_palettes_db(PalettesDB *DB){
  DB->db = palettedb_open(DB->Path);
  return((DB->db != NULL) ? 0 : 1);
}
