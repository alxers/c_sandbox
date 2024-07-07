#include <stdio.h>

int main() {
  int c;
  int space_count = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ') {
      ++space_count;
      continue;
    }

    if (space_count) {
      space_count = 0;
      putchar(' ');
      putchar(c);
    } else {
      putchar(c);
    }
  }

  return 0;
}