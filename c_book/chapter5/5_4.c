#include <stdio.h>

int strend(char *s, char *t) {
  int is_equal = 0;
  int t_counter = 0;
  while (*s != '\0') {
    if (*s == *t) {
      s++;
      t++;
      t_counter++;
      is_equal = 1;
    } else {
      s++;
      t -= t_counter;
      is_equal = 0;
    }
  }

  return is_equal;
}

int main() {

  char a[] = "Hello";
  char b[] = "Helloworld";
  char c[] = "llo";

  char d[] = "Hellowllo";

  printf("Result - Hello and llo: %d\n", strend(a, c));
  printf("Result - Helloworld and llo: %d\n", strend(b, c));
  printf("Result - Hellowllo and llo: %d\n", strend(d, c));
  return 0;
}