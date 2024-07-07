#include <stdio.h>

int main() {

  int empty_char = 0;
  int tab_char = 0;
  int end_of_line_char = 0;
  int c;
  while ((c = getchar()) != EOF) {
    if (c == ' ') {
      ++empty_char;
    } else if (c == '\t') {
      ++tab_char;
    } else if (c == '\n') {
      ++end_of_line_char;
    }
  }

  printf("Empty: %d, tab: %d, eol: %d\n", empty_char, tab_char, end_of_line_char);

  return 0;
}