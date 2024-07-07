#include <stdio.h>

int main() {

  char c;
  int lim = 5;
  // for (int i = 0; i < lim-1 && (c=getchar()) != '\n' && c != EOF; i++) {
  //   printf("%c\n", c);
  // }

  for (int i = 0; i < lim-1; i++) {
    if (c = getchar()) {
      if (c != '\n') {
        if (c != EOF) {
          printf("%c\n", c);
        }
      }
    }
  }

  return 0;
}