#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
  size_t width = 0, /* max length of line in pattern file */
      height = 0;   /* number of pattern file lines excluding comments */
  size_t length = 0, read;
  char *ptr, c;
  bool in_string = false;
  bool in_comment = false;
  // size_t pattern_begin = 0; /* seek point */

  /* TODO: it allows mixing comments and pattern lines which is not claimed as
     legitimate format in https://conwaylife.com/wiki/Plaintext
     If we cope with realloc some previous buffers if width changed, we can
     implement one-pass giving pattern data, not only counting measures.
     It's also required to get width of the first pattern line before allocation which is doable so:
      1. skip comments, store file position of pattern start
      2. look for '\n' counting bytes
      3. seek back to pattern start
      4. process file in one-pass
     */
  while ((read = fread (line, 1, PLAINTEXT_BUF_SIZE, text)))
    {
      ptr = line;
      while (ptr < line + read)
        {
          c = *(ptr++);
          if (c == '\n' || c == '\r')
            {
              if (in_string)
                {
                  in_string = false;
                  if (!in_comment)
                    {
                      height++;
                      width = MAX (width, length);
                    }
                }
            }
          else
            {
              if (!in_string)
                {
                  length = 1;
                  in_string = true;
                  in_comment = c == '!';
                }
              else
                length++;
            }
        }
    }

  if (!feof (text) || ferror (text))
    {
      perror ("plaintext");
      exit (errno);
    }

  printf ("max_line_len: %lu\n", width);
  printf ("linenum: %lu\n", height);

  // TODO: now init BYTESBUFFER with this params

  free (line);

  return NULL;
}

int Plaintext_get (BYTESBUFFER_T *buf, size_t x, size_t y);
