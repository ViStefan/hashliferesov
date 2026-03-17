#ifndef QUADROTREE_H
#define QUADROTREE_H 1

#include <stddef.h>

#include "typedefs.h"
#define LEAF 4

/*
    0 | 1
    -----
    2 | 3
*/

struct QTree
{
  union
  {
    QTREE *leaf[LEAF];
    // nodes of level 0 could represent real cells
    int bits;
  } data;
  int level;
};

QTREE *QTree_init (int depth, HASH *hash);
void QTree_print (QTREE *tree, int limit);
size_t QTree_hash (QTREE *node);
// void QTree_free(QTREE *tree);

#endif
