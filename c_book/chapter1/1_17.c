#include <stdio.h>

#define MAXLINE 1000

int getline(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    s[i] = c;
  }

  if (c == '\n') {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
  return i;
}

int main() {
  int len;
  char line[MAXLINE];

  int max = 0;

  while ((len = getline(line, MAXLINE)) > 0) {
    if (len > 5) {
      printf("len: %d, line: %s", len, line);
    }
  }

  return 0;
}