#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int is_all_lower = 1;
  int is_all_upper = 1;
  // Originally it should be argv[0] - the name of the program
  // but for testing purposes it's argv[1]
  if (argv[1]) {
    char *w = argv[1];
    while (*w != '\0') {
      if (*w >= 'A' && *w <= 'Z') {
        is_all_lower = 0;
      } else if (*w >= 'a' && *w <= 'z') {
        is_all_upper = 0;
      }
      w++;
    }
  }

  int c;
  while ((c = getchar()) != EOF) {
    if (is_all_lower) {
      printf("%c", tolower(c));
    } else if (is_all_upper) {
      printf("%c", toupper(c));
    } else {
      printf("%c", c);
    }
  }
  printf("\n");
}