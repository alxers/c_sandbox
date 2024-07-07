#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_STR_LEN 10

int is_non_graphic_char(int c) {
  if (c == '\t' || c == '\b' || c == ' ') {
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {

  int max_str_len_count = 0;

  int c;
  while ((c = getchar()) != EOF) {
    max_str_len_count++;
    if (max_str_len_count >= MAX_STR_LEN) {
      max_str_len_count = 0;
      printf("\n");
    }

    if (is_non_graphic_char(c)) {
      printf("%x", c);
    } else {
      printf("%c", c);
    }
  }
  printf("\n");
}

/* Test cases

Non graph chars tab, backspace, lines feed, null, blackslash
*/