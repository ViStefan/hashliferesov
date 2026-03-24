#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#include "plaintext.h"
#define PLAINTEXT_BUF_SIZE 8

typedef enum
{
  START = 0,
  COMMENT,
  NL_COMMENT,
  PATTERN,
  NL_PATTERN,
  ERROR
} STATE;

typedef struct STMT
{
  STATE state; /*statement; value from enum*/
  bool stay;  /*flag indicating whether to read new char*/
} STMT;

static void
_update_sizes (size_t *length, size_t *width, size_t *height)
{
  (*height)++;
  *width = MAX (*length, *width);
  *length = 0;
}

static void
_read_pattern_chunk (STMT *stmt, char c, size_t *length, size_t *width,
                     size_t *height)
{
  switch (c)
    {
    case '.':
    case 'O':
      (*length)++;
      stmt->state = PATTERN;
      break;

    case '\r':
      stmt->state = NL_PATTERN;
      _update_sizes (length, width, height);
      break;

    case '\n':
      stmt->state = PATTERN;
      _update_sizes (length, width, height);
      break;

    default:
      stmt->state = ERROR;
    }
}

static void
_check_newline (STMT *stmt, char c)
{
  stmt->stay = (c != '\n');
  if (stmt->state == NL_PATTERN)
    {
      stmt->state = PATTERN;
    }
  else if (stmt->state == NL_COMMENT)
    {
      stmt->state = START;
    }
}

static void
_read_comment_chunk (STMT *stmt, char c)
{
  switch (c)
    {
    case '\r':
      stmt->state = NL_COMMENT;
      break;
    case '\n':
      stmt->state = START;
      break;
    default:
      stmt->state = COMMENT;
      break;
    }
}

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

  STMT *stmt = (STMT *)malloc (sizeof (STMT)); /*statement struct for FSM*/
  stmt->state = START;
  stmt->stay = false;

  while ((read = fread (line, 1, PLAINTEXT_BUF_SIZE, text)))
    {
      ptr = line;
      stmt->stay = false;

      while (ptr < line + read)
        {
          c = *ptr;
          if (!stmt->stay)
            ptr++;
          stmt->stay = false;

          switch (stmt->state)
            {
            case START:
              switch (c)
                {
                case '!':
                  stmt->state = COMMENT;
                  break;
                case '.':
                case 'O':
                case '\n':
                case '\r':
                  stmt->state = PATTERN;
                  stmt->stay = true;
                  break;
                default:
                  stmt->state = ERROR;
                  break;
                }
              break;

            case COMMENT:
              _read_comment_chunk (stmt, c);
              break;

            case PATTERN:
              _read_pattern_chunk (stmt, c, &length, &width, &height);
              break;

            case NL_COMMENT:
            case NL_PATTERN:
              _check_newline (stmt, c);
              break;

            case ERROR:
              fclose (text);
              errno = EINVAL;
              perror ("plaintext");
              exit (errno);
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

  fclose (text);
  free (stmt);
  free (line);

  return NULL;
}

int Plaintext_get (BYTESBUFFER_T *buf, size_t x, size_t y);
