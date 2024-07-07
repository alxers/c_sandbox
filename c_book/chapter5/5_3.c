#include <stdio.h>

void old_strcat(char s[], char t[]) {
  int i, j;
  i = j = 0;

  while (s[i] != '\0') {
    i++;
  }

  while ((s[i++] = t[j++]) != '\0')
    ;
}

void strcat(char *s, char *t) {
  while (*s != '\0') {
    s++;
  }

  while ((*s++ = *t++) != '\0')
    ;
}

int main() {

  char a[6] = "He";
  char b[4] = "llo";

  printf("before, a: %s\n", a);

  // old_strcat(a, b);
  strcat(a, b);

  printf("after, a: %s\n", a);

  return 0;
}