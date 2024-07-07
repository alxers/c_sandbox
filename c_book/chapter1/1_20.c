#include <stdio.h>

#define MAXLINE 100
#define TAB_LEN 3

int getline(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    if (c == '\t') {
      int j;
      for (j = 0; j < TAB_LEN; j++) {
        s[i+j] = ' ';
      }
      i = i + (j - 1);
    } else {
      s[i] = c;
    }
  }

  if (c == '\n' && i != 0) {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
  printf("%s\n", s);
  return i;
}

void copy(char to[], char from[]) {
  int i = 0;

  while((to[i] = from[i]) != '\0') {
    ++i;
  }
}

int main() {
  int len;
  char line[MAXLINE];
  char longest[MAXLINE];

  int max = 0;

  while ((len = getline(line, MAXLINE)) > 0) {
    if (len > max) {
      max = len;
      copy(longest, line);
    }
  }

  printf("len: %d, line: %s", max, longest);

  return 0;
}