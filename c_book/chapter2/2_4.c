#include <stdio.h>

void squeeze(char s1[], char s2[]) {
  int n = 0;
  int outer_continue = 0;

  for (int i = 0; s1[i] != '\0'; i++) {
    for (int j = 0; s2[j] != '\0'; j++) {
      if (s1[i] == s2[j]) {
        outer_continue = 1;
        break;
      } else {
        outer_continue = 0;
      }
    }
    if (outer_continue) {
      continue;
    } else {
      s1[n++] = s1[i];
      outer_continue = 0;
    }
  }

  s1[n] = '\0';
  printf("%s\n", s1);
}

int main() {

  squeeze("hello", "el");
  squeeze("he", "e");
  squeeze("hello", "e");
  squeeze("hello", "m");
  squeeze("hello", "hello");
  squeeze("aaa", "bbb");
  squeeze("helloe", "e");
  squeeze("ehelloe", "e");

  return 0;
}