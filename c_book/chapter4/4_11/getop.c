#include <stdio.h>
#include <ctype.h>
#include "calc.h"
#include "is_eof.h"

int getop(char s[]) {
  int i, c;
  static int new_buf;
  static int new_buf_valid;

  while (1) {
    if (new_buf_valid == 1) {
      s[0] = c = new_buf;
      new_buf_valid = 0;
    } else {
      s[0] = c = getch();
    }

    if (c == ' ' || c == '\t') {
      continue;
    } else {
      break;
    }
  }

  s[1] = '\0';

  if (!isdigit(c) && c != '.')
    return c; /* not a number */
  i = 0;

  if (isdigit(c)) /* collect integer part */
    while (1) {
      if (new_buf_valid == 1) {
        s[++i] = c = new_buf;
        new_buf_valid = 0;
      } else {
        s[++i] = c = getch();
      }

      if (isdigit(c)) {
        continue;
      } else {
        break;
      }
    }

  if (c == '.') /* collect fraction part */
    while (1) {
      if (new_buf_valid == 1) {
        s[++i] = c = new_buf;
        new_buf_valid = 0;
      } else {
        s[++i] = c = getch();
      }

      if (isdigit(c)) {
        continue;
      } else {
        break;
      }
    }
  s[i] = '\0';

  if (c != EOF) {
    new_buf = c;
    new_buf_valid = 1;
  }

  return NUMBER;
}