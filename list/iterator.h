#pragma once
#include "../submodules/djbhash/src/djbhash.h"
#define DJBHASH_TABLE    "palettes_hash"
struct djbhash_node *item;
struct djbhash      hash, palettes_hash;

#define INIT_HASH_ITERATOR()              { do {                                      \
                                              djbhash_reset_iterator(&DJBHASH_TABLE); \
                                              item = djbhash_iterate(&DJBHASH_TABLE); \
                                            } while (0); }

#define INIT_HASH()                       { do {                            \
                                              djbhash_init(&DJBHASH_TABLE); \
                                              INIT_HASH_ITERATOR();         \
                                            } while (0); }

#define LOAD_HASH_TABLE()                 { do {                                                                 \
                                              INIT_HASH();                                                       \
                                              for (int i = 0; i < tbl_qty; i++) {                                \
                                                djbhash_set(&tbl_hash, tbl[i].filename, &tbl[i], DJBHASH_OTHER); \
                                              }                                                                  \
                                            } while (0); }

#define TBL_ITEM_INDEX_FILENAME(INDEX)    tbl[INDEX].filename

#define DEBUG_TBL_ITEM_SIZE(INDEX) \
  fprintf(stderr, "%lu\n", tbl[INDEX].size);

#define DEBUG_TBL_ITEM_NAME(INDEX) \
  fprintf(stderr, "%s\n", tbl[INDEX].filename);

#define DEBUG_TBL_ITEM(INDEX)                { do{                                                                                                          \
                                                 fprintf(stderr, "Item #%d:\t|%6lu bytes| Filename: %20s|\n", INDEX, tbl[INDEX].size, tbl[INDEX].filename); \
                                               } while (0); }

#define ITERATE_HASH(HASH_INDEX_FUNCTION)    { do {                                                                                                        \
                                                 INIT_HASH_ITERATOR();                                                                                     \
                                                 for (int HASH_INDEX = 0; HASH_INDEX < tbl_qty && item; HASH_INDEX++, item = djbhash_iterate(&tbl_hash)) { \
                                                   HASH_INDEX_FUNCTION(HASH_INDEX);                                                                        \
                                                 }                                                                                                         \
                                               } while (0); }

