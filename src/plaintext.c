#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "plaintext.h"

#define PLAINTEXT_BUF_SIZE 8

BYTESBUFFER_T *
Plaintext_read (char *path)
{
  FILE *text = fopen (path, "r");
  if (text == NULL)
    {
      perror ("plaintext");
      exit (errno);
    }

  char *line = (char *)malloc (PLAINTEXT_BUF_SIZE);
  size_t linenum = 0;
  size_t max_line_len = 0;
  size_t current_line_len = 0;
  bool trail = false;
  bool pattern = false;
  // size_t pattern_begin = 0; /* seek point */

  while (fgets (line, PLAINTEXT_BUF_SIZE, text))
    {
      if (pattern)
        {
          if (!trail)
            {
            pattern:
              current_line_len = strcspn (line, "\n");
              trail = (current_line_len == PLAINTEXT_BUF_SIZE - 1);
            }
          else
            {
              int cspn = strcspn (line, "\n");
              current_line_len += cspn;
              trail = (cspn == PLAINTEXT_BUF_SIZE);
            }
          if (!trail)
            {
              ++linenum;
              max_line_len = MAX (max_line_len, current_line_len);
            }
        }
      else
        {
          if (!trail)
            {
              pattern = (line[0] != '!');
              if (pattern)
                goto pattern;
              trail = (strcspn (line, "\n") == PLAINTEXT_BUF_SIZE - 1);
            }
          else
            trail = (strcspn (line, "\n") == PLAINTEXT_BUF_SIZE - 1);
        }
    }

  printf("max_line_len: %lu\n", max_line_len);
  printf("linenum: %lu\n", linenum);

  // TODO: now init BYTESBUFFER with this params

  free (line);

  return NULL;
}

int Plaintext_get (BYTESBUFFER_T *buf, size_t x, size_t y);
