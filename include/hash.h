#ifndef HASH_H
#define HASH_H 1

#include <stddef.h>

#include "typedefs.h"
#define TABLE_SIZE 4096

#ifndef KEY_T_DEFINED
#define KEY_T_DEFINED 1
typedef unsigned long KEY_T;
#endif

typedef struct Pair PAIR;
typedef size_t (*HASH_FUNC)(KEY_T);

HASH *Hash_init(HASH_FUNC function);
void Hash_insert(HASH *hash, KEY_T key, void *value);
int Hash_remove(HASH *hash, KEY_T key);
void *Hash_get(HASH *hash, KEY_T key);
size_t Hash_size(HASH *hash);

#endif
