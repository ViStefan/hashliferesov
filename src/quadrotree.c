#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "quadrotree.h"
#include "typedefs.h"

QTREE *
QTree_init (int depth, HASH *hash)
{
  assert (depth > 0);

  QTREE *tree, *leaf;

  tree = (QTREE *)malloc (sizeof (QTREE));
  tree->data.bits = 0;
  tree->level = 0;

  Hash_insert (hash, tree, tree);

  for (int l = 1; l < depth; l++)
    {
      leaf = tree;
      tree = (QTREE *)malloc (sizeof (QTREE));
      tree->level = l;

      Hash_insert (hash, tree, tree);

      for (int i = 0; i < LEAF; i++)
        tree->data.leaf[i] = leaf;
    }

  return tree;
}

void
_print_impl (QTREE *root, int tabs, int x, int y, int limit)
{
  printf ("%d: (%d, %d) x %d\n", root->level, x, y, 1 << root->level);
  for (int i = 0; i < LEAF; i++)
    {
      if (root->level > limit)
        {
          for (int t = 0; t < tabs + 1; t++)
            printf ("\t");
          printf ("%d: ", i);

          const int size = 1 << root->data.leaf[i]->level;
          const int _x = x + size * (i % 2);
          const int _y = y + size * (i > 1);
          _print_impl (root->data.leaf[i], tabs + 1, _x, _y, limit);
        }
    }
}

void
QTree_print (QTREE *tree, int limit)
{
  assert (limit >= 0);

  _print_impl (tree, 0, 0, 0, limit);
}

size_t
QTree_hash (QTREE *node)
{
  if (node->level)
    {
      size_t index = 1;
      for (int i = 0; i < LEAF; i++)
        index *= (unsigned long)node->data.leaf[i];
      return index;
    }
  else
    return (size_t)node->level;
}

// void QTree_free(QTREE *tree);
