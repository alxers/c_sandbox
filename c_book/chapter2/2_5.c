#include <stdio.h>

int any(char s1[], char s2[]) {
  int n = 0;

  for (int i = 0; s1[i] != '\0'; i++) {
    for (int j = 0; s2[j] != '\0'; j++) {
      if (s1[i] == s2[j]) {
        return i;
      }
    }
  }

  return -1;
}

int main() {

  printf("%d\n", any("hello", "el"));
  printf("%d\n", any("hello", "e"));
  printf("%d\n", any("hello", "m"));
  printf("%d\n", any("hello", "hello"));
  printf("%d\n", any("aaa", "bbb"));
  printf("%d\n", any("helloe", "e"));
  printf("%d\n", any("ehelloe", "e"));
  printf("%d\n", any("ehelloe", "al"));

  return 0;
}