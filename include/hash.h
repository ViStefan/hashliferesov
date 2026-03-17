#ifndef HASH_H
#define HASH_H 1

#include <stddef.h>
#define TABLE_SIZE 4096

typedef unsigned long KEY;
typedef struct Hash HASH;
typedef struct Pair PAIR;
typedef size_t (*HASH_FUNC)(KEY);

HASH *Hash_init(HASH_FUNC function);
void Hash_insert(HASH *hash, KEY key, void *value);
int Hash_remove(HASH *hash, KEY key);
void *Hash_get(HASH *hash, KEY key);
size_t Hash_size(HASH *hash);

#endif
